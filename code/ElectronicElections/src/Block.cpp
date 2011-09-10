/*
 * Block.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "Block.h"
#include <string.h>
#include <exception>

Block::Block(int size) : maxSize(size)
{
	this->bytes = new char[this->maxSize];
	memset(this->bytes, 0, this->maxSize);
	this->freeSpace = this->maxSize;
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
	memcpy(&occupiedSize, this->bytes, 4);
	this->freeSpace = this->maxSize - occupiedSize;

    // first four bytes in each record represent record size

	// ignore the first four bytes
	int sum = 4;
	int records = 0;
	while(sum < occupiedSize)
	{
		int recordSize;
		memcpy(&recordSize, this->bytes + sum, 4);
		sum += recordSize;
		records++;
	}

	this->recordCount = records;
}

int Block::getFreeSpace()
{
	return this->freeSpace;
}

void Block::forceInsert(Record *rec)
{
    int recSize = rec->getSize();
    int occupiedSpace = this->maxSize - this->freeSpace;
    // update block data
    memcpy(this->bytes + occupiedSpace, rec->getBytes(), recSize);
    // update block size
    occupiedSpace += recSize;
    memcpy(this->bytes, &occupiedSpace, 4);
}

void Block::insertRecord(Record *rec)
{
	if (!this->canInsertRecord(rec))
	{
		throw std::exception();
	}

	//TODO: Check that the record is unique
    this->forceInsert(rec);
}

bool Block::canInsertRecord(Record *rec)
{
	return this->freeSpace >= rec->getSize();
}

Block::~Block()
{
	delete[] this->bytes;
}
