/*
 * RLVBlock.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "SimpleVariableBlock.h"
#include <string.h>
#include <exception>

SimpleVariableBlock::SimpleVariableBlock(int size, RecordMethods* methods) :
BaseVariableBlock(size, Constants::BLOCK_HEADER_SIZE, Constants::BLOCK_HEADER_SIZE, methods)
{
}

void SimpleVariableBlock::updateInformation()
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
		short recordSize;
		memcpy(&recordSize, this->bytes + sum, Constants::RECORD_HEADER_SIZE);
		sum += (recordSize + Constants::RECORD_HEADER_SIZE);
		records++;
	}

	this->recordCount = records;
}

bool SimpleVariableBlock::hasNextRecord()
{
	return this->position < this->getOccupiedSize();
}

VariableRecord* SimpleVariableBlock::getNextRecord(VariableRecord* r)
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

void SimpleVariableBlock::clear()
{
	memset(this->bytes, 0, this->maxSize);
	this->position = Constants::BLOCK_HEADER_SIZE;
	this->freeSpace = this->maxSize;
}

void SimpleVariableBlock::printContent()
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

UpdateResult SimpleVariableBlock::updateRecord(const char* key, VariableRecord* rec)
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

	if (this->canInsertRecord(sizeDifference))
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
		memcpy(this->bytes, &occupiedSpace, Constants::BLOCK_HEADER_SIZE);
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

void SimpleVariableBlock::forceInsert(VariableRecord *rec)
{
    short recSize = rec->getSize();
    int occupiedSpace = this->getOccupiedSize();

    if (occupiedSpace == 0)
    {
    	occupiedSpace += Constants::BLOCK_HEADER_SIZE;
    }
    // add record size
    memcpy(this->bytes + occupiedSpace, &recSize, Constants::RECORD_HEADER_SIZE);
    occupiedSpace += Constants::RECORD_HEADER_SIZE;
    // add record data
    memcpy(this->bytes + occupiedSpace, rec->getBytes(), recSize);
    occupiedSpace += recSize;
    // update block size
    memcpy(this->bytes, &occupiedSpace, Constants::BLOCK_HEADER_SIZE);
}

bool SimpleVariableBlock::insertRecord(const char* key, VariableRecord *rec)
{
	if (!this->canInsertRecord(rec->getSize()) || this->findRecord(key, &rec) >= 0)
	{
		return false;
	}
    this->forceInsert(rec);
    this->updateInformation();
    return true;
}

bool SimpleVariableBlock::canInsertRecord(int recordSize)
{
	return this->freeSpace >= recordSize + Constants::FIELD_HEADER_SIZE;
}

bool SimpleVariableBlock::removeRecord(const char* key)
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
	memcpy(this->bytes, &occupiedSpace, Constants::BLOCK_HEADER_SIZE);

	this->updateInformation();
	if (r != NULL)
	{
		delete r;
	}
	return true;
}

SimpleVariableBlock::~SimpleVariableBlock()
{
}
