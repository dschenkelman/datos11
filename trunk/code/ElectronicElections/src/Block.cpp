/*
 * Block.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "Block.h"
#include <string.h>
#include <exception>

Block::Block(int size) : maxSize(size), position(0)
{
	this->bytes = new char[this->maxSize];
	memset(this->bytes, 0, this->maxSize);
	this->freeSpace = this->maxSize;
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
    // add record size
    memcpy(this->bytes + occupiedSpace, &recSize, 4);

    // add record data
    memcpy(this->bytes + occupiedSpace, rec->getBytes(), recSize);

    // update block size
    occupiedSpace += (recSize + 4);
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

void Block::copyBlock(const Block & other)
{
    this->freeSpace = other.freeSpace;
    this->maxSize = other.maxSize;
    this->recordCount = other.recordCount;
    this->position = other.position;
    this->bytes = new char[this->maxSize];
    memset(this->bytes, 0, this->maxSize);
    memcpy(this->bytes, other.bytes, this->maxSize);
}

Block::~Block()
{
	delete[] this->bytes;
}
