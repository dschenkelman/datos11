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
	:Block(size, recordSize, methods, BLOCK_POINTER_SIZE)
{
}

bool TreeBlock::insertRecord(char* key, char* b)
{
	Record r(this->recordSize);
	if (!this->canInsertRecord() || this->findRecord(key, &r) >= 0)
	{
		return false;
	}

	int recordIndex = this->findFirstFreeRecord();
	short flagIndex = this->getFlagByteFromRecordIndex(recordIndex);
	// update record bytes
	memcpy(this->bytes + (recordIndex * this->recordSize + this->flagBytes),
			b, this->recordSize);
	// update empty control flag
	char emptyBitIndex = (recordIndex % 4) * 2 + 1;
	ByteOperators::setBit(this->bytes + flagIndex, emptyBitIndex, 1);

	this->occupiedRecords++;
	bool forward = false;

	while(this->getNextRecord(&r) != NULL &&
			this->recordMethods->compare(key,
					r.getBytes(), this->recordSize) > 0)
	{
		forward = true;
		// swap
		memcpy(this->bytes + (recordIndex * this->recordSize + this->flagBytes),
				this->bytes + ((this->currentRecord - 1) * this->recordSize + this->flagBytes),
				this->recordSize);
		memcpy(this->bytes + ((this->currentRecord - 1) * this->recordSize + this->flagBytes),
						b, this->recordSize);

		recordIndex = this->currentRecord - 1;
	}

	if (forward)
	{
		return true;
	}

	this->currentRecord = recordIndex;
	while(this->getPreviousRecord(&r) != NULL &&
			this->recordMethods->compare(key,
					r.getBytes(), this->recordSize) < 0)
	{
		// swap
		memcpy(this->bytes + (recordIndex * this->recordSize + this->flagBytes),
				this->bytes + (this->currentRecord * this->recordSize + this->flagBytes),
				this->recordSize);
		memcpy(this->bytes + (this->currentRecord * this->recordSize + this->flagBytes),
						b, this->recordSize);

		recordIndex = this->currentRecord;
	}

	return true;
}

void TreeBlock::setNextBlock(int blockNumber)
{
	memcpy(this->bytes + (this->maxSize - BLOCK_POINTER_SIZE),
			&blockNumber, BLOCK_POINTER_SIZE);
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

TreeBlock::~TreeBlock()
{
}
