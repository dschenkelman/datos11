/*
 * SequenceTreeBlock.cpp
 *
 *  Created on: Sep 25, 2011
 *      Author: damian
 */

#include "SequenceTreeBlock.h"
#include <string.h>
#include "../Blocks/Constants.h"
#include <vector>

using namespace std;

SequenceTreeBlock::SequenceTreeBlock(int size, RecordMethods *methods)
: TreeBlock(size,RECORD_OFFSET, RECORD_OFFSET, methods)
{
	this->updateFreeSpace(RECORD_OFFSET);
	this->recordsOffset = RECORD_OFFSET;
}

void SequenceTreeBlock::updateInformation()
{
	memcpy(&this->freeSpace, this->bytes, Constants::BLOCK_HEADER_SIZE);
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

void SequenceTreeBlock::updateFreeSpace(int space)
{
    this->freeSpace = space;
    memcpy(this->bytes + TreeBlock::LEVEL_SIZE, &this->freeSpace, TreeBlock::FREE_SPACE_SIZE);
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

bool SequenceTreeBlock::insertRecord(const char *key, VariableRecord *rec)
{
	if (!this->canInsertRecord(rec->getSize()) || this->findRecord(key, &rec) >= 0)
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

	this->forceInsert(rec);

    // order
    VariableRecord aux;

    while(this->recordMethods->compare(key,aux.getBytes(), aux.getSize()) < 0)
    {
    	// swap
    	memcpy(this->bytes + recordPositions.at(recordPositions.size())
    			+ rec->size(), aux.getBytes(), aux.getSize());

    	memcpy(this->bytes + recordPositions.at(recordPositions.size())
    			, rec->getBytes(), rec->getSize());

		// change to previous record
    	recordPositions.pop_back();
		this->position = recordPositions.at(recordPositions.size());
		this->getNextRecord(&aux);
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

VariableRecord *SequenceTreeBlock::getNextRecord(VariableRecord *r)
{
	if (!this->hasNextRecord())
	{
		return NULL;
	}

	short recordSize;
	memcpy(&recordSize, this->bytes + this->position, Constants::RECORD_HEADER_SIZE);
	this->position += Constants::RECORD_HEADER_SIZE;
	r->setBytes(this->bytes + this->position, recordSize);
	this->position += recordSize;

	return r;
}

bool SequenceTreeBlock::hasNextRecord()
{
	return this->position < this->getOccupiedSize();
}

bool SequenceTreeBlock::insertSeparator(const char *key, short  len)
{
	return false;
}

bool SequenceTreeBlock::removeSeparator(const char *key)
{
	return false;
}

void SequenceTreeBlock::positionAtBegin()
{
	this->position = this->RECORD_OFFSET;
}

SequenceTreeBlock::~SequenceTreeBlock()
{
}
