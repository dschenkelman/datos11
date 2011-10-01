/*
 * HashBlock.cpp
 *
 *  Created on: Sep 22, 2011
 *      Author: damian
 */

#include "HashBlock.h"
#include "../Helpers/ByteOperators.h"
#include <math.h>
#include <string.h>

HashBlock::HashBlock(int size, RecordMethods* methods)
: BaseVariableBlock(size, 5, 5, methods)
{
}

bool HashBlock::isEmpty()
{
	int occupiedSize;
	memcpy(&occupiedSize, this->bytes + 1, sizeof(int));
	return this->maxSize == (this->freeSpace);
}

int HashBlock::getOverflowedBlock()
{
	char overflowedBlock;
	memcpy(&overflowedBlock, this->bytes, sizeof(char));
	return overflowedBlock;
}

void HashBlock::setNoOverflow()
{
	char noOverflow = -1;
	memcpy(this->bytes, &noOverflow, sizeof(char));
}

void HashBlock::becomesOverflow(char ovflowBlock)
{
	memcpy(this->bytes, &ovflowBlock, sizeof(char));
}

void HashBlock::updateInformation()
{
	int occupiedSize;
	memcpy(&occupiedSize, this->bytes + 1, sizeof(int));
	this->freeSpace = this->maxSize - occupiedSize;
}

bool HashBlock::hasNextRecord()
{
	return this->position < this->getOccupiedSize();
}

VariableRecord* HashBlock::getNextRecord(VariableRecord* r)
{
	if (!this->hasNextRecord())
	{
		return NULL;
	}
	short recordSize;
	memcpy(&recordSize, this->bytes + this->position, sizeof(short));
	this->position += sizeof(short);
	r->setBytes(this->bytes + this->position, recordSize);
	this->position += recordSize;
	return r;
}

void HashBlock::clear()
{
	memset(this->bytes, 0, this->maxSize);
	char nonOverflowReference = -1;
	memcpy(this->bytes, &nonOverflowReference, sizeof(char));
	this->position = this->recordsOffset;
	this->freeSpace = this->maxSize - this->recordsOffset;
}

void HashBlock::printContent()
{
	this->position = this->recordsOffset;
	VariableRecord* record = new VariableRecord();
	while(this->getNextRecord(record) != NULL)
	{
		this->recordMethods->print(record->getBytes(), record->getSize());
		delete record;
		record = new VariableRecord();
	}
	delete record;
}

void HashBlock::forceInsert(VariableRecord *rec)
{
    short recSize = rec->getSize();
    int occupiedSpace = this->getOccupiedSize();

    if (occupiedSpace == 0)
    {
    	occupiedSpace += this->recordsOffset;
    }
    // add record size
    memcpy(this->bytes + occupiedSpace, &recSize, sizeof(short));
    occupiedSpace += sizeof(short);
    // add record data
    memcpy(this->bytes + occupiedSpace, rec->getBytes(), recSize);
    occupiedSpace += recSize;
    // update block size
    memcpy(this->bytes +1, &occupiedSpace, sizeof(int));
}

bool HashBlock::insertRecord(const char* key, VariableRecord *rec)
{
	if (!this->canInsertRecord(rec->getSize()) || this->findRecord(key, &rec) >= 0)
	{
		return false;
	}
    this->forceInsert(rec);
    this->updateInformation();
    return true;
}


UpdateResult HashBlock::updateRecord(const char* key, VariableRecord* rec)
{
	VariableRecord* r = new VariableRecord();
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
		char buffer[bufferSize];
		memset(buffer, 0, bufferSize);

		// copy bytes that are after record
		memcpy(buffer, this->bytes + this->position, bufferSize);

		// update record
		short recordSize = rec->getSize();
		memcpy(this->bytes + startPosition, &recordSize, sizeof(short));
		memcpy(this->bytes + (startPosition + sizeof(short)), rec->getBytes(), recordSize);

		// add
		memcpy(this->bytes + (startPosition + sizeof(short) + recordSize), buffer, bufferSize);

		// update block size
		memcpy(this->bytes +1, &occupiedSpace, sizeof(int));
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

bool HashBlock::removeRecord(const char* key)
{
	VariableRecord* r = new VariableRecord();
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
	if(occupiedSpace == this->recordsOffset) //block is empty!
		occupiedSpace = 0;
	memcpy(this->bytes +1, &occupiedSpace, Constants::BLOCK_HEADER_SIZE);

	this->updateInformation();
	if (r != NULL)
	{
		delete r;
	}
	return true;
}

HashBlock::~HashBlock()
{
}

