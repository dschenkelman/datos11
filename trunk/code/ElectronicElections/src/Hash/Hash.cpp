#include "Hash.h"
#include "HashBlock.h"


Hash::Hash(HashBlockFile* file, int blockAmount)
{
	this->file = file;
	this->blockAmount = blockAmount;
}

int Hash::hashFunction(char* key)
{
	// This will change
	//int hash = atoi(key) % blockAmount;
	//return hash;
	return 0;
}

bool Hash::insertRecord(char* key, Record* record)
{
	int blockNumber = this->hashFunction(key);
	int firstErasedRecordPtr[2];	// [BlockNumber, RecordNumber]
	bool firstErasedFound = false;
	int lastBlockToCheck;
	
	//If the block does not exist, Blockfile must creates it
	
	if ( blockNumber == 0 )
	{
		lastBlockToCheck = blockAmount;
	}
	else
	{
		lastBlockToCheck = blockNumber - 1;
	}
	
	file->loadBlock(blockNumber);
	HashBlock* block = file->getCurrentBlock();
	if( block->findRecord(key, record) >= 0)	//the record was found
	{
		return false; 
	}
	
	while (blockNumber != lastBlockToCheck)
	{
		file->loadBlock(blockNumber);
		block = file->getCurrentBlock();
		if( block->findRecord(key, record) >= 0)	//the record was found
		{
			return false; 
		}
		//the record is not in the current block
		else if (block->isFull() || block->isOverflowed())
		{
			if (block->isFull())
			{
				block->getsOverflow();
				file->saveBlock();
			}
			else if ( !firstErasedFound )
			{
				//Save first erased record
				firstErasedFound = true;
				int occupiedRecords = 0;
				block->seekRecord(occupiedRecords);
				Record* currentRecord = block->getCurrentRecord(currentRecord);
				while( !currentRecord->getWasDeleted() && block->hasNextRecord())
				{
					currentRecord = block->getNextRecord(currentRecord);
					occupiedRecords++;
				}
				if (currentRecord->getWasDeleted())
				{
					firstErasedRecordPtr[0] = blockNumber;
					firstErasedRecordPtr[1] = occupiedRecords;
				}
			}
		}
		//the current block has a free record
		else
		{
			if (firstErasedFound)
			{
				file->loadBlock(firstErasedRecordPtr[0]);
				block = file->getCurrentBlock();
				block->seekRecord(firstErasedRecordPtr[1]);
				block->insertInCurrentRecord(key,record->getBytes());
			}
			else
				block->insertRecord(key,record->getBytes());
			file->saveBlock();
			return true;
		}
		blockNumber++;
		if (blockNumber == blockAmount)
			blockNumber = 0;
	}
	//No more space in blockFile!!
	return false;
}
