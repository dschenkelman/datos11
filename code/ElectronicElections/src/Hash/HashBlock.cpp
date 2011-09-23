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

HashBlock::HashBlock(int size, int recordSize, RecordMethods* methods)
: Block(size, recordSize, methods)
{
	this->flagBytes = ceil((this->recordCount) / 4.0);

	while ((flagBytes + this->recordCount * this->recordSize) > this->maxSize)
	{
		this->recordCount--;
	}
}

bool HashBlock::insertRecord(char* key, char* b)
	//	insert record in the first available position.
	// 	verify that it is unique
{
	// just for hash
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


bool HashBlock::updateRecord(const char* key, char* b)
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

bool HashBlock::removeRecord(const char* key)
	//remove the record that matches the key
{
	// just for hash
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

Record* HashBlock::getCurrentRecord(Record* r)
{
	// just valid for hash
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

int HashBlock::findFirstFreeRecord()
{
	// just for hash
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


bool HashBlock::insertInCurrentRecord(char* key, char* b)
	//	insert record in the first Current Record.
{
	Record* rec = new Record(16);
	if (! this->getCurrentRecord(rec)->getIsEmpty() )
	{
		delete [] rec;
		return false;
	}
	short flagIndex = this->getFlagByteFromRecordIndex(this->currentRecord);
	// update record bytes
	memcpy(this->bytes + (this->currentRecord * this->recordSize + this->flagBytes), b, this->recordSize);
	// update empty control flag
	char emptyBitIndex = (this->currentRecord % 4) * 2 + 1;
	ByteOperators::setBit(this->bytes + flagIndex, emptyBitIndex, 1);

	this->occupiedRecords++;
	delete [] rec;
    return true;
}

short HashBlock::getFlagByteFromRecordIndex(int recordIndex)
{
	// just for hash
	return (short)(recordIndex * 2) / 8;
}

HashBlock::~HashBlock()
{
}
