/*
 * SequenceTreeBlock.cpp
 *
 *  Created on: Sep 25, 2011
 *      Author: damian
 */

#include "SequenceTreeBlock.h"
#include <string.h>
#include "../VariableBlocks/Constants.h"
#include <vector>

using namespace std;

SequenceTreeBlock::SequenceTreeBlock(int size, RecordMethods *methods, bool existing)
: TreeBlock(size,RECORD_OFFSET, RECORD_OFFSET, methods), nextNode(0)
{
	if (!existing)
	{
		this->updateFreeSpace(size - RECORD_OFFSET);
	}
}

void SequenceTreeBlock::updateInformation()
{
	memcpy(&this->freeSpace, this->bytes + TreeBlock::LEVEL_SIZE, TreeBlock::FREE_SPACE_SIZE);
	this->level = this->getLevel();
}

void SequenceTreeBlock::printContent()
{
	this->position = RECORD_OFFSET;
	VariableRecord* record = new VariableRecord();
	while(this->getNextRecord(record) != NULL)
	{
		this->recordMethods->print(record->getBytes(), record->getSize());
		delete record;
		record = new VariableRecord();
	}

	delete record;
}

void SequenceTreeBlock::clear()
{
	memset(this->bytes + this->RECORD_OFFSET, 0, this->maxSize);
	this->position = RECORD_OFFSET;
    this->updateFreeSpace(this->maxSize - this->RECORD_OFFSET);
}

void SequenceTreeBlock::forceInsert(VariableRecord *rec)
{
	short recSize = rec->getSize();
	int occupiedSpace = this->getOccupiedSize();

	// add record size
	memcpy(this->bytes + occupiedSpace, &recSize, Constants::RECORD_HEADER_SIZE);
	occupiedSpace += Constants::RECORD_HEADER_SIZE;
	// add record data
	memcpy(this->bytes + occupiedSpace, rec->getBytes(), recSize);
	occupiedSpace += recSize;
	// update block size
	this->updateFreeSpace(this->maxSize - occupiedSpace);
}

bool SequenceTreeBlock::removeRecord(const char *key)
{
	VariableRecord* r = NULL;
	int startPosition = this->findRecord(key, &r);

	if (startPosition < 0)
	{
		if (r != NULL)
		{
			delete r;
		}
		return false;
	}

	short recordSize = this->position - (startPosition + Constants::RECORD_HEADER_SIZE);
	int occupiedSpace = this->getOccupiedSize();
	occupiedSpace -= (recordSize + Constants::RECORD_HEADER_SIZE);

	int bufferSize = this->maxSize - this->position;
	char buffer[bufferSize];
	memset(buffer, 0, bufferSize);

	// copy bytes that are after record
	memcpy(buffer, this->bytes + this->position, bufferSize);

	// free space of record from end and remove record
	memset(this->bytes + startPosition, 0, bufferSize + recordSize + Constants::RECORD_HEADER_SIZE);
	memcpy(this->bytes + startPosition, buffer, bufferSize);

	// update block size
	this->updateFreeSpace(this->maxSize - occupiedSpace);

	this->updateInformation();
	if (r != NULL)
	{
		delete r;
	}
	return true;
}

bool SequenceTreeBlock::canInsertRecord(int size)
{
	return this->freeSpace >= size + Constants::RECORD_HEADER_SIZE;
}

bool SequenceTreeBlock::insertRecord(VariableRecord* keyRecord, VariableRecord* dataRecord)
{
	if (!this->canInsertRecord(dataRecord->getSize()) || this->findRecord(keyRecord->getBytes(), &dataRecord) >= 0)
	{
		return false;
	}

	vector<int> recordPositions;

	this->position = RECORD_OFFSET;
	while(this->position < this->getOccupiedSize())
	{
		recordPositions.push_back(this->position);
		short recordSize;
		memcpy(&recordSize, this->bytes + this->position, Constants::RECORD_HEADER_SIZE);
		this->position += Constants::RECORD_HEADER_SIZE + recordSize;
	}

	this->forceInsert(dataRecord);

    // order
    if (this->getOccupiedSize() > RECORD_OFFSET + dataRecord->getSize() + Constants::RECORD_HEADER_SIZE)
    {
		VariableRecord aux;
		this->position = recordPositions.at(recordPositions.size() - 1);
		this->getNextRecord(&aux);

		while(this->recordMethods->compare(keyRecord->getBytes(),aux.getBytes(), aux.getSize()) < 0)
		{
			// swap
			short auxSize = aux.getSize();
			short recSize = dataRecord->getSize();

			memcpy(this->bytes + recordPositions.at(recordPositions.size() - 1)
								+ dataRecord->getSize() + Constants::RECORD_HEADER_SIZE, &auxSize,
								Constants::RECORD_HEADER_SIZE);

			memcpy(this->bytes + recordPositions.at(recordPositions.size() - 1)
					+ dataRecord->getSize() + 2 * Constants::RECORD_HEADER_SIZE, aux.getBytes(), auxSize);

			memcpy(this->bytes + recordPositions.at(recordPositions.size() - 1)
					, &recSize, Constants::RECORD_HEADER_SIZE);

			memcpy(this->bytes + recordPositions.at(recordPositions.size() - 1) + Constants::RECORD_HEADER_SIZE
								, dataRecord->getBytes(), recSize);

			// change to previous record
			recordPositions.pop_back();
			if (recordPositions.size() == 0)
			{
				break;
			}

			this->position = recordPositions.at(recordPositions.size() - 1);
			this->getNextRecord(&aux);
		}
    }

    this->updateInformation();
    return true;
}

UpdateResult SequenceTreeBlock::updateRecord(const char *key, VariableRecord *rec)
{
	VariableRecord* r = NULL;
	int startPosition = this->findRecord(key, &r);

	if (startPosition < 0)
	{
		if (r != NULL)
		{
			delete r;
		}
		return NOT_FOUND;
	}

	short sizeDifference = rec->getSize() - r->getSize();
	int occupiedSpace = this->getOccupiedSize();

	if (this->canInsertRecord(sizeDifference - Constants::RECORD_HEADER_SIZE))
	{
		occupiedSpace += sizeDifference;
		// there is enough space to perform the update
		// in the current block
		int bufferSize = (this->maxSize - this->position) - sizeDifference;
		char buffer[bufferSize];
		memset(buffer, 0, bufferSize);

		// copy bytes that are after record
		memcpy(buffer, this->bytes + this->position, bufferSize);

		// update record
		short recordSize = rec->getSize();
		memcpy(this->bytes + startPosition, &recordSize, Constants::RECORD_HEADER_SIZE);
		memcpy(this->bytes + (startPosition + Constants::RECORD_HEADER_SIZE), rec->getBytes(), recordSize);

		// add
		memcpy(this->bytes + (startPosition + Constants::RECORD_HEADER_SIZE + recordSize), buffer, bufferSize);

		// update block size
		this->updateFreeSpace(this->maxSize - occupiedSpace);
	}
	else
	{
		// move to another block
		this->removeRecord(key);
		if (r != NULL)
		{
			delete r;
		}
		return INSUFFICIENT_SPACE;
	}

	if (r != NULL)
	{
		delete r;
	}
	return UPDATED;
}

bool SequenceTreeBlock::hasNextRecord()
{
	return this->position < this->getOccupiedSize();
}

void SequenceTreeBlock::positionAtBegin()
{
	this->position = this->RECORD_OFFSET;
}

void SequenceTreeBlock::insertNodePointer(int index, int node)
{
}

void SequenceTreeBlock::updateNodePointer(int index, int node)
{
}

void SequenceTreeBlock::removeNodePointer(int index)
{
}

int SequenceTreeBlock::getNodePointer(int index)
{
	return -1;
}

int SequenceTreeBlock::getNextNode()
{
	memcpy(&this->nextNode, this->bytes + TreeBlock::FREE_SPACE_SIZE + TreeBlock::LEVEL_SIZE, NEXT_NODE_SIZE);
	return this->nextNode;
}

void SequenceTreeBlock::setNextNode(int node)
{
	this->nextNode = node;
	memcpy(this->bytes + TreeBlock::FREE_SPACE_SIZE + TreeBlock::LEVEL_SIZE, &this->nextNode, NEXT_NODE_SIZE);
}

VariableRecord* SequenceTreeBlock::popFirst()
{
	this->position = RECORD_OFFSET;
	short recordSize;
	memcpy(&recordSize, this->bytes + this->position, Constants::RECORD_HEADER_SIZE);
	this->position += Constants::RECORD_HEADER_SIZE;
	char record[recordSize];
	memcpy(record, this->bytes + this->position, recordSize);
	VariableRecord* ret = new VariableRecord(record,recordSize);
	this->removeRecord(this->recordMethods->getKeyRecord(record,recordSize)->getBytes());
	return ret;
}

VariableRecord *SequenceTreeBlock::popLast()
{
	VariableRecord aux;
	this->positionAtBegin();
	while (this->getNextRecord(&aux) != NULL){}

	VariableRecord* ret = new VariableRecord;
	*ret = aux;
	this->removeRecord(this->recordMethods->getKeyRecord(aux.getBytes(),aux.getSize())->getBytes());

	return ret;
}

SequenceTreeBlock::~SequenceTreeBlock()
{
}
