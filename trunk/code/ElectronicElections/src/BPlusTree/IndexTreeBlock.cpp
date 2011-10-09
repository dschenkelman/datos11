/*
 * IndexTreeBlock.cpp
 *
 *  Created on: Sep 27, 2011
 *      Author: juanma
 */

#include "IndexTreeBlock.h"
#include <string.h>
#include <vector>

using namespace std;

IndexTreeBlock::IndexTreeBlock(int size, RecordMethods* methods, bool existing)
: TreeBlock(size,RECORD_OFFSET,RECORD_OFFSET,methods), nodesPosition(RECORD_OFFSET)
{
	if (!existing)
	{
		this->updateFreeSpace(this->freeSpace);
	}
}

void IndexTreeBlock::clear()
{
	memset(this->bytes + this->RECORD_OFFSET, 0, this->maxSize - RECORD_OFFSET);
	this->position = RECORD_OFFSET;
    this->updateFreeSpace(this->maxSize - RECORD_OFFSET);
    this->nodesPosition = RECORD_OFFSET;
    this->storeNodesPosition();
}

bool IndexTreeBlock::canInsertRecord(int size)
{
	return this->freeSpace >= size + Constants::RECORD_HEADER_SIZE + NODE_POINTER_SIZE;
}

bool IndexTreeBlock::hasNextRecord()
{
	return this->position < this->nodesPosition;
}

void IndexTreeBlock::forceInsert(VariableRecord *rec)
{
	short recSize = rec->getSize();

	// copy node pointers
	int bufferSize = this->maxSize - this->nodesPosition;
	char buffer[bufferSize];
	memcpy(buffer, this->bytes + this->nodesPosition, bufferSize);

	// add record size
	memcpy(this->bytes + this->nodesPosition, &recSize, Constants::RECORD_HEADER_SIZE);
	this->freeSpace -= Constants::RECORD_HEADER_SIZE;
	this->nodesPosition += Constants::RECORD_HEADER_SIZE;
	// add record data
	memcpy(this->bytes + this->nodesPosition, rec->getBytes(), recSize);
	this->freeSpace -= recSize;
	this->nodesPosition += recSize;

	// restore node pointers
	memcpy(this->bytes + this->nodesPosition, buffer,
			bufferSize - (recSize + Constants::RECORD_HEADER_SIZE));

	this->storeNodesPosition();

	// update block size
	this->updateFreeSpace(this->freeSpace);
}

bool IndexTreeBlock::insertRecord(VariableRecord* keyRecord, VariableRecord* dataRecord)
{
	// data record has the key separator to be inserted.
	if (!this->canInsertRecord(dataRecord->getSize()) ||
			this->findRecord(keyRecord->getBytes(), &dataRecord) >= 0)
	{
		return false;
	}

	vector<int> recordPositions;

	this->position = RECORD_OFFSET;
	while(this->position < this->nodesPosition)
	{
		recordPositions.push_back(this->position);
		short recordSize;
		memcpy(&recordSize, this->bytes + this->position, Constants::RECORD_HEADER_SIZE);
		this->position += Constants::RECORD_HEADER_SIZE + recordSize;
	}

	this->forceInsert(dataRecord);

    // order
    if (recordPositions.size() > 0)
    {
		VariableRecord aux;
		this->position = recordPositions.at(recordPositions.size() - 1);
		this->getNextRecord(&aux);

		while(this->recordMethods->compare
				(keyRecord->getBytes(),aux.getBytes(), aux.getSize()) < 0)
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

void IndexTreeBlock::storeNodesPosition()
{
	memcpy(this->bytes + TreeBlock::LEVEL_SIZE + TreeBlock::FREE_SPACE_SIZE,
			&this->nodesPosition, FIRST_POINTER_POSITION);
}

void IndexTreeBlock::loadNodesPosition()
{
	memcpy(&this->nodesPosition,
			this->bytes + TreeBlock::LEVEL_SIZE + TreeBlock::FREE_SPACE_SIZE,
			FIRST_POINTER_POSITION);
}

void IndexTreeBlock::updateInformation()
{
	memcpy(&this->freeSpace, this->bytes + TreeBlock::LEVEL_SIZE, Constants::BLOCK_HEADER_SIZE);
	this->level = this->getLevel();
	this->loadNodesPosition();
}

bool IndexTreeBlock::removeRecord(const char *key)
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
	this->nodesPosition -= (recordSize + Constants::RECORD_HEADER_SIZE);
	this->storeNodesPosition();

	this->updateInformation();
	if (r != NULL)
	{
		delete r;
	}
	return true;
}

void IndexTreeBlock::printContent()
{
}

UpdateResult IndexTreeBlock::updateRecord(const char *key, VariableRecord *rec)
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

		this->nodesPosition += sizeDifference;
		this->storeNodesPosition();
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

void IndexTreeBlock::updateNodePointer(int index, int node)
{
	int startPosition = this->nodesPosition + NODE_POINTER_SIZE * index;
	// update value
	memcpy(this->bytes + startPosition, &node, NODE_POINTER_SIZE);
}

void IndexTreeBlock::insertNodePointer(int index, int node)
{
	this->freeSpace -= NODE_POINTER_SIZE;
	int startPosition = this->nodesPosition + NODE_POINTER_SIZE * index;

	// buffer end of block
	int bufferSize = this->maxSize - startPosition;
	char buffer[bufferSize];
	memcpy(buffer, this->bytes + startPosition, bufferSize);

	// insert value
	memcpy(this->bytes + startPosition, &node, NODE_POINTER_SIZE);

	// restore old values
	memcpy(this->bytes + startPosition + NODE_POINTER_SIZE, buffer, bufferSize - NODE_POINTER_SIZE);

	this->updateFreeSpace(this->freeSpace);
}

void IndexTreeBlock::removeNodePointer(int index)
{
	this->freeSpace += NODE_POINTER_SIZE;
	int startPosition = this->nodesPosition + NODE_POINTER_SIZE * index;

	// buffer end of block
	int bufferSize = this->maxSize - (startPosition + NODE_POINTER_SIZE);
	char buffer[bufferSize];
	memcpy(buffer, this->bytes + startPosition + NODE_POINTER_SIZE, bufferSize);

	// delete value
	memset(this->bytes + startPosition, 0, bufferSize + NODE_POINTER_SIZE);
	memcpy(this->bytes + startPosition, buffer, bufferSize);

	this->updateFreeSpace(this->freeSpace);
}

int IndexTreeBlock::getNodePointer(int index)
{
	int value = 0;
	int startPosition = this->nodesPosition + NODE_POINTER_SIZE * index;
	memcpy(&value, this->bytes + startPosition, NODE_POINTER_SIZE);

	return value;
}

void IndexTreeBlock::setNextNode(int node)
{
}

int IndexTreeBlock::getNextNode()
{
	return -1;
}

VariableRecord* IndexTreeBlock::popFirst()
{
	// TODO if needed
	return NULL;
}

VariableRecord *IndexTreeBlock::popLast()
{
	return NULL;
}

IndexTreeBlock::~IndexTreeBlock()
{
}

