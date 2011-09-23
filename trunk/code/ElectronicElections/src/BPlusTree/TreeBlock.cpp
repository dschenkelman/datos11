/*
 * TreeBlock.cpp
 *
 *  Created on: Sep 20, 2011
 *      Author: damian
 */

#include "TreeBlock.h"
#include <string.h>
#include "../Helpers/ByteOperators.h"

TreeBlock::TreeBlock(int size, int recordSize, RecordMethods* methods)
	:Block(size, recordSize, methods), nextBlockBytes(BLOCK_POINTER_SIZE)
{
	while ((RECORDS_OFFSET + this->recordCount * this->recordSize) > this->maxSize)
	{
		this->recordCount--;
	}

	this->loadMetadata();
}

bool TreeBlock::insertRecord(char* key, char* b)
{
	Record r(this->recordSize);
	if (!this->canInsertRecord() || this->findRecord(key, &r) >= 0)
	{
		return false;
	}

	// update record bytes
	memcpy(this->bytes + RECORDS_OFFSET + (this->occupiedRecords * this->recordSize),
			b, this->recordSize);

	int recordIndex = this->occupiedRecords;
	this->currentRecord = recordIndex;

	while(this->getPreviousRecord(&r) != NULL &&
			this->recordMethods->compare(key, r.getBytes(), this->recordSize) < 0)
	{
		// swap
		memcpy(this->bytes + (recordIndex * this->recordSize + RECORDS_OFFSET),
					this->bytes + (this->currentRecord * this->recordSize + RECORDS_OFFSET),
					this->recordSize);
		memcpy(this->bytes + (this->currentRecord * this->recordSize + RECORDS_OFFSET),
						b, this->recordSize);

		recordIndex = this->currentRecord;
	}

	this->updateOccupiedRecords(+1);
	return true;
}

Record *TreeBlock::getPreviousRecord(Record *r)
{
        if (this->currentRecord <= 0)
        {
                return NULL;
        }

        do
        {
                this->currentRecord--;
                this->getCurrentRecord(r);
        } while(r->getIsEmpty());
        return r;
}

Record *TreeBlock::getCurrentRecord(Record *r)
{

	memcpy(r->getBytes(),
			this->bytes + RECORDS_OFFSET + this->currentRecord * this->recordSize,
			this->recordSize);

	r->setIsEmpty(false);
	r->setWasDeleted(false);

	return r;
}

bool TreeBlock::updateRecord(const char *key, char *b)
{
	Record r(this->recordSize);
	int recordIndex;

	if ((recordIndex = this->findRecord(key, &r) < 0))
	{
		return false;
	}

	// update record bytes
	memcpy(this->bytes + ((recordIndex * this->recordSize) + RECORDS_OFFSET),
			b, this->recordSize);

	return true;
}

bool TreeBlock::removeRecord(const char *key)
{
	Record r(this->recordSize);
	int recordIndex;

	if ((recordIndex = this->findRecord(key, &r) < 0))
	{
		return false;
	}

	// update record bytes
	int startBuffer = RECORDS_OFFSET + ((recordIndex + 1) * this->recordSize);

	char buffer[this->maxSize - startBuffer];


	memcpy(buffer, this->bytes + startBuffer, this->maxSize - startBuffer);

	memset(this->bytes + RECORDS_OFFSET + (recordIndex * this->recordSize),
			0, this->maxSize - startBuffer + this->recordSize);

	memcpy(this->bytes + RECORDS_OFFSET + (recordIndex * this->recordSize),
			buffer, this->maxSize - startBuffer);

	this->updateOccupiedRecords(-1);

	return true;
}


void TreeBlock::setNextBlock(int blockNumber)
{
	memcpy(this->bytes, &blockNumber, BLOCK_POINTER_SIZE);
}


int TreeBlock::getNextBlock()
{
	int value;
	memcpy(&value, this->bytes, BLOCK_POINTER_SIZE);
	return value;
}

void TreeBlock::setLevel(short value)
{
	memcpy(this->bytes + BLOCK_POINTER_SIZE, &value, LEVEL_SIZE);
}

short TreeBlock::getLevel()
{
	short value;
	memcpy(&value, this->bytes + BLOCK_POINTER_SIZE, LEVEL_SIZE);
	return value;
}

void TreeBlock::updateOccupiedRecords(char diff)
{
	this->occupiedRecords += diff;

	memcpy(this->bytes + BLOCK_POINTER_SIZE + LEVEL_SIZE, &this->occupiedRecords, COUNT_SIZE);
}


int TreeBlock::retrieveOccupiedRecords()
{
	int value;
	memcpy(&value, this->bytes + BLOCK_POINTER_SIZE + LEVEL_SIZE, COUNT_SIZE);
	return value;
}

void TreeBlock::loadMetadata()
{
	this->occupiedRecords = this->retrieveOccupiedRecords();
}


TreeBlock::~TreeBlock()
{
}
