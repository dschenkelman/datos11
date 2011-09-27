/*
 * HashBlockFile.cpp
 *
 *  Created on: Sep 22, 2011
 *      Author: Gaby+JM
 *

#include "HashBlockFile.h"

HashBlockFile::HashBlockFile(std::string& fileName, int bSize,
		int rSize, RecordMethods *methods, bool createNew, int blockAmount)
: BlockFile (fileName, bSize, rSize, methods, createNew)
{
	this->blockAmount = blockAmount;
	this->currentBlock = new HashBlock(bSize, methods);
	this->initializefile();
}

HashBlockFile::HashBlockFile(string& name, int bSize, RecordMethods* methods, int blockamount, bool createNew)
			:blockSize(bSize), blockAmount(blockamount)
{
	//first block in file:
	//long for blocksize, long for blockamount, all longs for occupied space in each block.
	this->fileName = name;
	this->recordMethods = methods;
	this->positionToDataBlocks = new char[bSize-4];
	this->currentBlock = new HashBlock(this->blockSize, this->recordMethods);
	if (createNew)
	{
		this->dataFile.open(this->fileName.c_str(), ios::binary | ios::in | ios::out | ios::trunc);
		char* initialValue= new char[bSize * 2]; //to start with an empty block
		memset(initialValue, 0, bSize *2);
		memset(this->positionToDataBlocks, 0, bSize-4);
		this->blockAmount = 1;
		memcpy(initialValue, &blockAmount, sizeof(long));
		this->dataFile.seekp(0, ios::beg);
		this->dataFile.write(initialValue, this->blockSize * 2);
		delete [] initialValue;
	}
	else
	{
		this->dataFile.open(this->fileName.c_str(), ios::binary | ios::in | ios::out);
		char* bytes = new char[this->blockSize];
		this->dataFile.read(bytes, this->blockSize);
		memcpy(&this->blockAmount, bytes, 4);
		memcpy(this->positionToDataBlocks, bytes + 4, this->blockSize - 4);
		delete [] bytes;
	}
}

HashBlock* HashBlockFile::getCurrentBlock()
{
	return this->currentBlock;
}

HashBlockFile::~HashBlockFile()
{
	delete this->currentBlock;
}

void HashBlockFile::initializefile()
{
	char* block = new char [blockSize];
	memset(block,0,blockSize);
	memcpy(block,&(-1),sizeof(char));
	for (int i= 0; i < blockAmount; i++)
	{
		this->dataFile.seekp(i*blockSize);
		this->dataFile.write(block,blockSize);
	}

}


bool HashBlockFile::insertRecord(char* key, VariableRecord* record)
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


int HashBlockFile::hashFunction(char* key)
{
	// This will change
	//int hash = atoi(key) % blockAmount;
	//return hash;
	return 0;
}
*/
