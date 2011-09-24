/*
 * RLVBlock.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "RLVBlock.h"
#include <string.h>
#include <exception>

RLVBlock::RLVBlock(int size, RecordMethods* methods) : maxSize(size),
position(Constants::BLOCK_HEADER_SIZE)
{
	this->bytes = new char[this->maxSize];
	memset(this->bytes, 0, this->maxSize);
	this->freeSpace = this->maxSize;
	this->recordMethods = methods;
}

RLVBlock& RLVBlock::operator =(const RLVBlock & other)
{
	if (this == &other)
	{
		return *this;
	}

	this->copyBlock(other);

	return *this;
}

RLVBlock::RLVBlock(RLVBlock& other)
{
    this->copyBlock(other);
}

char* RLVBlock::getBytes()
{
	return this->bytes;
}

int RLVBlock::getRecordCount()
{
	return this->recordCount;
}

void RLVBlock::updateInformation()
{
	int occupiedSize;
	memcpy(&occupiedSize, this->bytes, Constants::BLOCK_HEADER_SIZE);
	this->freeSpace = this->maxSize - occupiedSize;

    // first four bytes in each record represent record size

	// ignore the first four bytes
	int sum = Constants::BLOCK_HEADER_SIZE;
	int records = 0;
	while(sum < occupiedSize)
	{
		int recordSize;
		memcpy(&recordSize, this->bytes + sum, Constants::RECORD_HEADER_SIZE);
		sum += (recordSize + Constants::RECORD_HEADER_SIZE);
		records++;
	}

	this->recordCount = records;
}

int RLVBlock::getFreeSpace()
{
	return this->freeSpace;
}

inline int RLVBlock::getOccupiedSize()
{
	return this->maxSize - this->freeSpace;
}

bool RLVBlock::hasNextRecord()
{
	return this->position < this->getOccupiedSize();
}

VariableRecord* RLVBlock::getNextRecord(VariableRecord* r)
{
	if (!this->hasNextRecord())
	{
		return NULL;
	}

	int recordSize;
	memcpy(&recordSize, this->bytes + this->position, Constants::RECORD_HEADER_SIZE);
	this->position += Constants::RECORD_HEADER_SIZE;
	r->setBytes(this->bytes + this->position, recordSize);
	this->position += recordSize;

	return r;
}

int RLVBlock::findRecord(const char* key, VariableRecord** rec)
{
	this->position = Constants::BLOCK_HEADER_SIZE;
	VariableRecord* record = new VariableRecord();
	int foundPosition = this->position;
	while(this->getNextRecord(record) != NULL)
	{
		if (this->recordMethods->compare(key,
				record->getBytes(), record->getSize()) == 0)
		{
			*rec = record;
			return foundPosition;
		}
		delete record;
		record = new VariableRecord();
		foundPosition = this->position;
	}

	delete record;
	return -1;
}

void RLVBlock::clear()
{
	memset(this->bytes, 0, this->maxSize);
	this->position = Constants::BLOCK_HEADER_SIZE;
	this->freeSpace = this->maxSize;
}

void RLVBlock::printContent()
{
	this->position = Constants::BLOCK_HEADER_SIZE;
	VariableRecord* record = new VariableRecord();
	while(this->getNextRecord(record) != NULL)
	{
		this->recordMethods->print(record->getBytes(), record->getSize());
		delete record;
		record = new VariableRecord();
	}

	delete record;
}

UpdateResult RLVBlock::updateRecord(const char* key, VariableRecord* rec)
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

	int sizeDifference = rec->getSize() - r->getSize();
	int occupiedSpace = this->getOccupiedSize();

	if (this->canInsertRecord(sizeDifference))
	{
		occupiedSpace += sizeDifference;
		// there is enough space to perform the update
		// in the current block
		int bufferSize = (this->maxSize - this->position) - sizeDifference;
		char* buffer = new char[bufferSize];
		memset(buffer, 0, bufferSize);

		// copy bytes that are after record
		memcpy(buffer, this->bytes + this->position, bufferSize);

		// update record
		int recordSize = rec->getSize();
		memcpy(this->bytes + startPosition, &recordSize, Constants::RECORD_HEADER_SIZE);
		memcpy(this->bytes + (startPosition + Constants::RECORD_HEADER_SIZE), rec->getBytes(), recordSize);

		// add
		memcpy(this->bytes + (startPosition + Constants::RECORD_HEADER_SIZE + recordSize), buffer, bufferSize);

		// update block size
		memcpy(this->bytes, &occupiedSpace, Constants::BLOCK_HEADER_SIZE);

		delete[] buffer;
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

void RLVBlock::forceInsert(VariableRecord *rec)
{
    int recSize = rec->getSize();
    int occupiedSpace = this->getOccupiedSize();

    if (occupiedSpace == 0)
    {
    	occupiedSpace += 4;
    }

    // add record size
    memcpy(this->bytes + occupiedSpace, &recSize, 4);
    occupiedSpace += 4;

    // add record data
    memcpy(this->bytes + occupiedSpace, rec->getBytes(), recSize);
    occupiedSpace += recSize;

    // update block size
    memcpy(this->bytes, &occupiedSpace, 4);
}

bool RLVBlock::insertRecord(VariableRecord *rec)
{
	if (!this->canInsertRecord(rec->getSize()))
	{
		return false;
	}

    this->forceInsert(rec);
    return true;
}

bool RLVBlock::canInsertRecord(int size)
{
	return this->freeSpace >= size;
}

void RLVBlock::copyBlock(const RLVBlock & other)
{
    this->freeSpace = other.freeSpace;
    this->maxSize = other.maxSize;
    this->recordCount = other.recordCount;
    this->position = other.position;
    this->bytes = new char[this->maxSize];
    memset(this->bytes, 0, this->maxSize);
    memcpy(this->bytes, other.bytes, this->maxSize);
    this->recordMethods = other.recordMethods;
}

bool RLVBlock::removeRecord(const char* key)
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

	int recordSize = this->position - (startPosition + Constants::RECORD_HEADER_SIZE);
	int occupiedSpace = this->getOccupiedSize();
	occupiedSpace -= (recordSize + Constants::RECORD_HEADER_SIZE);

	int bufferSize = this->maxSize - this->position;
	char* buffer = new char[bufferSize];
	memset(buffer, 0, bufferSize);

	// copy bytes that are after record
	memcpy(buffer, this->bytes + this->position, bufferSize);

	// free space of record from end and remove record
	memset(this->bytes + startPosition, 0, bufferSize + recordSize + Constants::RECORD_HEADER_SIZE);
	memcpy(this->bytes + startPosition, buffer, bufferSize);

	// update block size
	memcpy(this->bytes, &occupiedSpace, Constants::BLOCK_HEADER_SIZE);

	this->updateInformation();
	delete[] buffer;
	if (r != NULL)
	{
		delete r;
	}
	return true;
}

RLVBlock::~RLVBlock()
{
	delete[] this->bytes;
}
