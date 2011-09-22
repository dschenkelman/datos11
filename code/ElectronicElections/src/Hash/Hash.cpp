#include "Hash.h"

Hash::Hash(BlockFile* file, long blockAmount)
{
	this->file = file;
	this->blockAmount = blockAmount;
}

int Hash::hashFunction(long key)
{
	// This will change
	int hash = key % blockAmount;
	return hash;
}

bool Hash::insertRecord(long key, Record* record)
{
    int blockNumber = this->hashFunction(key);
	//If the block does not exist, Blockfile must creates it

	return internalInsertRecord(blockNumber,record);
}

bool Hash::internalInsertRecord(int blockNumber, Record* record)
{
	//If the block does not exist, Blockfile must creates it
	//Block* loadedBlock =
	file->loadBlock(blockNumber);
	Block* block = file->getCurrentBlock();



	if( block->isFull() )
	{
		//Block is already full
		if (blockNumber == blockAmount)
		{
			//The current block is the last one
			int firstBlockNumber = 1;
			return internalInsertRecord(firstBlockNumber,record);
		}
		return internalInsertRecord(blockNumber+1,record);
	}/*
	else if( block->isEmpty() )
	{
		block->insertRecord(record);
		file->saveBlock();
		return true;
	}

	//The loaded block is empty
	block->seek(0);
	int erasedRecord = 0;
	bool firstErasedFound = false;
	Record* currentRecord = block->getRecord();
	if( currentRecord->getWasDeleted() )
	{
		firstErasedFound = true;
	}
	while( block->hasNextRecord() )
	{
		if( !firstErasedFound ) erasedRecord++;

		currentRecord = block->getNextRecord();
		if( currentRecord->getIsEmpty() ) break;

		if( currentRecord->getWasDeleted() && !firstErasedFound ) firstErasedFound = true;
	}
	if( firstErasedFound )
	{
		block->seek(erasedRecord);
		block->insertRecord(record);
	}
	else block->insertRecord(record);
	if( block->isFull() )
	{
		//Block gets OVERFLOW!!
		block->getsOverflow();
	}
	file->saveBlock();
*/
	return true;
}
