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

Block::Block(int bs, int rs, RecordMethods* methods, char nextBlockBytes)
: maxSize(bs), occupiedRecords(0),recordSize(rs), nextBlockBytes(nextBlockBytes)
{
	this->recordCount = this->maxSize / this->recordSize;
	this->flagBytes = ceil((this->recordCount) / 4.0);

	while ((flagBytes + this->nextBlockBytes
			+ this->recordCount * this->recordSize) > this->maxSize)
	{
		this->recordCount--;
	}
	this->bytes = new char[this->maxSize];
	memset(this->bytes, 0, this->maxSize);
	this->recordMethods = methods;
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
Record* Block::getCurrentRecord(Record* r)
{
	short flagByte = (this->currentRecord * 2) / 8;
	int startingPosition = this->currentRecord * this->recordSize + this->flagBytes;
	char deletedBitIndex = (this->currentRecord % 4) * 2;
	r->setBytes(this->bytes + startingPosition);
	r->setWasDeleted(
			ByteOperators::isBitOne
				(this->bytes[flagByte], deletedBitIndex));
	r->setIsEmpty(
			!ByteOperators::isBitOne
				(this->bytes[flagByte], deletedBitIndex + 1));

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
	while(this->getNextRecord(rec) != NULL)
	{
		if (this->recordMethods->compare(key,
				rec->getBytes(), rec->getSize()) == 0)
		{
			return foundRecord;
		}
		foundRecord = this->currentRecord;
	}

	return -1;
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
		if (!record->getIsEmpty())
		{
			this->recordMethods->print(record->getBytes(), record->getSize());
		}
	}

	delete record;
}

bool Block::updateRecord(const char* key, char* b)
{
	Record r(this->recordSize);
	int recordIndex = this->findRecord(key, &r);

	if (recordIndex < 0)
	{
		return false;
	}

	// update record bytes
	memcpy(this->bytes + (recordIndex * this->recordSize + this->flagBytes), b, this->recordSize);
	return true;
}

bool Block::insertRecord(char* key, char* b)
	//	insert record in the first available position.
	// 	verify that it is unique
{
	Record r(this->recordSize);
	if (!this->canInsertRecord() || this->findRecord(key, &r) >= 0)
	{
		return false;
	}

	int recordIndex = this->findFirstFreeRecord();
	short flagIndex = this->getFlagByteFromRecordIndex(recordIndex);
	// update record bytes
	memcpy(this->bytes + (recordIndex * this->recordSize + this->flagBytes), b, this->recordSize);
	// update empty control flag
	char emptyBitIndex = (recordIndex % 4) * 2 + 1;
	ByteOperators::setBit(this->bytes + flagIndex, emptyBitIndex, 1);

	this->occupiedRecords++;
    return true;
}

bool Block::canInsertRecord()
{
	return this->occupiedRecords < this->recordCount;
}

bool Block::removeRecord(const char* key)
	//remove the record that matches the key
{
	if(!this->isEmpty())
	{
		Record rec(this->recordSize);
		int recordIndex = this->findRecord(key, &rec);
		if (recordIndex < 0)
		{
			return false;
		}
		short flagIndex = this->getFlagByteFromRecordIndex(recordIndex);
		char deleteBitIndex = (recordIndex % 4) * 2;
		// mark record as deleted
		ByteOperators::setBit(this->bytes + flagIndex, deleteBitIndex, 1);
		// mark record as free
		ByteOperators::setBit(this->bytes + flagIndex, deleteBitIndex + 1, 0);
		this->occupiedRecords--;
		return true;
	}

	return false;
}

int Block::findFirstFreeRecord()
{
	short byteIndex = 0;
	while (byteIndex < this->flagBytes)
	{
		char currentByte = this->bytes[byteIndex];
		char bit = 1;
		while (bit < 8)
		{
			if (!ByteOperators::isBitOne(currentByte, bit))
			{
				return (int)byteIndex * 4 + (int) bit / 2;
			}
			bit += 2;
		}

		byteIndex++;
	}

	return -1;
}

short Block::getFlagByteFromRecordIndex(int recordIndex)
{
	return (short)(recordIndex * 2) / 8;
}

Block::~Block()
{
	delete[] this->bytes;
}
