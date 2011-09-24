/*
 * Block.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "Block.h"
#include <math.h>
#include <string.h>
#include "Constants.h"
#include "../Helpers/ByteOperators.h"

Block::Block(int bs, int rs, RecordMethods* methods)
: maxSize(bs), occupiedRecords(0),recordSize(rs)
{
	this->recordCount = this->maxSize / this->recordSize;
	this->bytes = new char[this->maxSize];
	memset(this->bytes, 0, this->maxSize);
	this->recordMethods = methods;
	this->overflow = false;
}

char* Block::getBytes()
{
	return this->bytes;
}

bool Block::isOverflowed()
{
	return this->overflow;
}
void Block::getsOverflow()
{
	this->overflow = true;
}

int Block::getRecordCount()
{
	return this->recordCount;
}

bool Block::isEmpty()
{
	return (this->occupiedRecords == 0);
}

bool Block::isFull()
{
	return (this->occupiedRecords == this->recordCount);
}

bool Block::hasNextRecord()
{
	return this->currentRecord < this->occupiedRecords;
}

Record* Block::getNextRecord(Record* r)
{
	if (!this->hasNextRecord())
	{
		return NULL;
	}

	do
	{
		this->getCurrentRecord(r);
		this->currentRecord++;
	} while(r->getIsEmpty());
	return r;
}

void Block::seekRecord(int recordNumber)
{
	this->currentRecord = recordNumber;
}

int Block::findRecord(const char* key, Record* rec)
{
	this->seekRecord(0); //comienzo del 1er registro a buscar
	int foundRecord = this->currentRecord;
	Record* nextRecord = new Record(this->recordSize);
	while(this->getNextRecord(nextRecord) != NULL)
	{
		if (this->recordMethods->compare(key,
				nextRecord->getBytes(), nextRecord->getSize()) == 0)
		{
			return foundRecord;
		}
		foundRecord = this->currentRecord;
	}
	delete nextRecord;
	return -1;
}

int Block::getRecordSize()
{
	return this->recordSize;
}

void Block::clear()
{
	memset(this->bytes, 0, this->maxSize);
	this->occupiedRecords = 0;
	this->currentRecord = 0;
}

void Block::printContent()
{
	this->seekRecord(0);
	Record* record = new Record(this->recordSize);
	while(this->getNextRecord(record) != NULL)
	{
		if (!record->getIsEmpty() || this->occupiedRecords == 0)
		{
			this->recordMethods->print(record->getBytes(), record->getSize());
		}
	}

	delete record;
}

bool Block::canInsertRecord()
{
	return this->occupiedRecords < this->recordCount;
}

int Block::getOccupiedRecords()
{
	return this->occupiedRecords;
}

Block::~Block()
{
	delete[] this->bytes;
}
