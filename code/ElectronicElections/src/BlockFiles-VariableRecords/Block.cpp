/*
 * Block.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "Block.h"
#include <string.h>
#include <exception>
#include "Constants.h"

Block::Block(int size, RecordMethods* methods) : maxSize(size),
position(Constants::BLOCK_HEADER_SIZE)
{
	this->bytes = new char[this->maxSize];
	memset(this->bytes, 0, this->maxSize);
	this->freeSpace = this->maxSize;
	this->recordMethods = methods;
}

Block& Block::operator =(const Block & other)
{
	if (this == &other)
	{
		return *this;
	}

	this->copyBlock(other);

	return *this;
}

Block::Block(Block& other)
{
    this->copyBlock(other);
}

char* Block::getBytes()
{
	return this->bytes;
}

int Block::getRecordCount()
{
	return this->recordCount;
}

void Block::updateInformation()
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

int Block::getFreeSpace()
{
	return this->freeSpace;
}

inline int Block::getOccupiedSize()
{
	return this->maxSize - this->freeSpace;
}

bool Block::hasNextRecord()
{
	return this->position < this->getOccupiedSize();
}

Record* Block::getNextRecord(Record* r)
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

int Block::findRecord(char* key, Record* rec)int Block::findRecord(const char* key, Record** rec)
{
	this->position = Constants::BLOCK_HEADER_SIZE;
	Record* record = new Record();
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
		record = new Record();
		foundPosition = this->position;
	}

	delete record;
	return -1;
}

void Block::clear()
{
	memset(this->bytes, 0, this->maxSize);
	this->position = Constants::BLOCK_HEADER_SIZE;
	this->freeSpace = this->maxSize;
}

void Block::printContent()
{
	this->position = Constants::BLOCK_HEADER_SIZE;
	Record* record = new Record();
	while(this->getNextRecord(record) != NULL)
	{
		this->recordMethods->print(record->getBytes(), record->getSize());
		delete record;
		record = new Record();
	}

	delete record;
}

UpdateResult Block::updateRecord(const char* key, Record* rec)
{
	Record* r = NULL;
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

void Block::forceInsert(Record *rec)
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

bool Block::insertRecord(Record *rec)
{
	if (!this->canInsertRecord(rec->getSize()))
	{
		return false;
	}

    this->forceInsert(rec);
    return true;
}

bool Block::canInsertRecord(int size)
{
	return this->freeSpace >= size;
}

void Block::copyBlock(const Block & other)
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

bool Block::removeRecord(const char* key)
{
	Record* r = NULL;
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

Block::~Block()
{
	delete[] this->bytes;
}
