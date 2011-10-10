/*
 * HashBlockFile.cpp
 *
 *  Created on: Sep 22, 2011
 *      Author: Gaby+JM
 */

#include "HashBlockFile.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

HashBlockFile::HashBlockFile(std::string fileName, int bSize, RecordMethods *methods, HashingFunction* hashFunction, int blockAmount, bool createNew)
: BaseVariableBlockFile (fileName, bSize, methods)
{
	this->ovflowBlockUsed = false;
	this->hashBlockUsed = false;
	this->totalBlocks = blockAmount;
	this->hashingFunction = hashFunction;
	this->currentBlock = new HashBlock(bSize, methods);
	if( createNew)
	{
		this->initializefile();
	}
	else
	{
		this->dataFile.open(this->fileName.c_str(), ios::binary | ios::in | ios::out);
	}
	string ovflw = fileName + "ovflow";
	this->overflowFile = new SimpleVariableBlockFile(ovflw, 512, methods, createNew);
}

void HashBlockFile::initializefile()
{
	this->dataFile.open(this->fileName.c_str(), ios::binary | ios::in | ios::out | ios::trunc);
	char block[blockSize];
	memset(block,0,blockSize);
	char overflowBlock = -1;
	memcpy(block,&overflowBlock,sizeof(char));
	for (int i= 0; i < totalBlocks; i++)
	{
		this->positionAtBlock(i);
		this->dataFile.write(block,blockSize);
	}
}

HashBlock* HashBlockFile::getCurrentBlock()
{
	return this->currentBlock;
}

int HashBlockFile::fakeHashFunction(const char* key)
{
	// This will change
	int hash = rand() % this->totalBlocks;
	return 2;
}

void HashBlockFile::printContent()
{
	int blockNumber = 0;
	int ovflwBlock;
	this->positionAtBlock(0);
	while(!this->isAtEOF())
	{
		this->loadBlock(blockNumber);
		if(!this->currentBlock->isEmpty() )
		{
			this->currentBlock->printContent();
			std::cout << "" << std::endl;
		}
		if( (ovflwBlock = this->currentBlock->getOverflowedBlock()) != -1)
		{
			this->overflowFile->loadBlock(ovflwBlock);
			this->overflowFile->getCurrentBlock()->printContent();
			std::cout << "" << std::endl;
			while((ovflwBlock = this->overflowFile->getCurrentBlock()->getOverflowedBlock()) != -1)
			{
				this->overflowFile->loadBlock(ovflwBlock);
				this->overflowFile->getCurrentBlock()->printContent();
				std::cout << "" << std::endl;
			}
		}
		blockNumber++;
	}
}

void HashBlockFile::loadRecord(const char* key, VariableRecord* record)
{
	//int blockNumber = this->fakeHashFunction(key);
	//to use on each Record Type :D
	int blockNumber = this->hashingFunction->hashingFunction(key, this->totalBlocks);

	this->loadBlock(blockNumber);
	if(this->currentBlock->getFreeSpace() >= record->getSize() +2)
	{
		this->currentBlock->forceInsert(record);
		this->hashBlockUsed = true;
		this->saveBlock();
		return;
	}
	//hay overflow en hash block
	int overflowBlock;
	if((overflowBlock = this->currentBlock->getOverflowedBlock()) == -1)
	{
		overflowBlock = this->overflowFile->getFirstFreeEmptyBlock();
		this->currentBlock->becomesOverflow(overflowBlock);
		this->hashBlockUsed = true;
		this->overflowFile->loadBlock(overflowBlock);
		this->overflowFile->getCurrentBlock()->setNoOverflow();//initialize it with -1
		this->overflowFile->getCurrentBlock()->forceInsert(record);
		this->ovflowBlockUsed = true;
		this->saveBlock();
		return;
	}
	this->overflowFile->loadBlock(overflowBlock);
	int freeSpace = this->overflowFile->getCurrentBlock()->getFreeSpace();
	while(freeSpace < record->getSize()+2)
	{
		if((overflowBlock = this->overflowFile->getCurrentBlock()->getOverflowedBlock()) == -1)
		{
			overflowBlock = this->overflowFile->getFirstFreeEmptyBlock();
			this->ovflowBlockUsed = true;
			this->overflowFile->getCurrentBlock()->becomesOverflow(overflowBlock);
			this->saveBlock();//block came overflowed, modify block reference
			this->overflowFile->loadBlock(overflowBlock); //load new empty overflow block
			this->overflowFile->getCurrentBlock()->setNoOverflow();//initialize it with -1
		}
		else
			this->overflowFile->loadBlock(overflowBlock);
		freeSpace = this->overflowFile->getCurrentBlock()->getFreeSpace();
	}
	//found available space
	this->overflowFile->getCurrentBlock()->forceInsert(record);
	this->ovflowBlockUsed = true;
	this->saveBlock();
	return;
}

int HashBlockFile::getAvailableOverflowBlock(const char* key, VariableRecord* record)
{
	SimpleVariableBlock* block = this->overflowFile->getCurrentBlock();
	int freeSpace = block->getFreeSpace();
	char ovflwBlock = block->getOverflowedBlock();
	char freeBlock = ovflwBlock;
	while(freeSpace < record->getSize()+2)
	{
		if(ovflwBlock == -1)
		{
			freeBlock = this->overflowFile->getFirstFreeEmptyBlock();
			block->becomesOverflow(freeBlock);
			this->ovflowBlockUsed = true;
			this->saveBlock();
			this->overflowFile->loadBlock(freeBlock);
			this->overflowFile->getCurrentBlock()->setNoOverflow();
			this->ovflowBlockUsed = true;
			this->saveBlock();
			break;
		}
		this->overflowFile->loadBlock(ovflwBlock);
		block = this->overflowFile->getCurrentBlock();
		freeBlock = ovflwBlock;
		ovflwBlock = block->getOverflowedBlock();
		freeSpace = block->getFreeSpace();
	}
	return freeBlock;
}

bool HashBlockFile::insertRecord(const char* key, VariableRecord* record)
{
	//int blockNumber = this->fakeHashFunction(key);
	//to use on each Record Type :D
	int blockNumber = this->hashingFunction->hashingFunction(key, this->totalBlocks);

	if(blockNumber >= totalBlocks)
		return false; //out of bounds. It should never happen!!

	int ovflowBlock;
	this->loadBlock(blockNumber);
	this->currentBlock = this->getCurrentBlock();
	if(this->currentBlock->findRecord(key, &record) >= 0)
	{
		//ERROR, KEY ALREADY INSERTED
		return false;
	}
	if(this->currentBlock->canInsertRecord(record->getSize()) )
	{
		if( (ovflowBlock = this->currentBlock->getOverflowedBlock()) != -1)
		{
			this->overflowFile->loadBlock(ovflowBlock);
			if(this->findInOverflowBlocks(key, &record, false) != -1)
			{
				return false;
			}
		}
		this->hashBlockUsed = true;
		this->currentBlock->insertRecord(key, record);
		this->saveBlock();
		return true;
	}
	//no space in block, overflow!
	if((ovflowBlock = this->currentBlock->getOverflowedBlock()) != -1)
	{
		this->overflowFile->loadBlock(ovflowBlock);
		if(this->findInOverflowBlocks(key, &record, false) != -1)
		{
			return false;
		}
		this->overflowFile->loadBlock(ovflowBlock);
		if(!this->overflowFile->getCurrentBlock()->insertRecord(key, record) )
		{
			//overflow block is full. check on overflowed block
			if((ovflowBlock = this->getAvailableOverflowBlock(key, record)) == -1)
				return false; //key was found
			this->overflowFile->loadBlock(ovflowBlock);
			this->overflowFile->getCurrentBlock()->insertRecord(key, record);
		}
		this->ovflowBlockUsed = true;
		this->saveBlock();
		return true;
	}
	//it wasn't overflowed. now it is
	this->hashBlockUsed = true;
	this->ovflowBlockUsed = true;
	ovflowBlock = this->overflowFile->getFirstFreeEmptyBlock();
	this->currentBlock->becomesOverflow(ovflowBlock);
	this->overflowFile->loadBlock(ovflowBlock);
	this->overflowFile->getCurrentBlock()->insertRecord(key, record);
	this->saveBlock();
	return true;
}

bool HashBlockFile::getRecord(const char* key, VariableRecord** record)
{
	//int blockNumber = this->fakeHashFunction(key);
	//to use on each Record Type :D
	int blockNumber = this->hashingFunction->hashingFunction(key, this->totalBlocks);


	int overflwBlock;
	this->loadBlock(blockNumber);
	if(this->currentBlock->findRecord(key, record) != -1)
		return true;
	if((overflwBlock = this->currentBlock->getOverflowedBlock() ) != -1)
	{
		this->overflowFile->loadBlock(overflwBlock);
		if(this->findInOverflowBlocks(key, record, true) != -1)
			return true;
	}
	return false;
}

bool HashBlockFile::updateRecord(const char *key, VariableRecord* record)
{
	UpdateResult result;
	//int blockNumber = this->fakeHashFunction(key);
	//to use on each Record Type :D
	int blockNumber = this->hashingFunction->hashingFunction(key, this->totalBlocks);

	if(blockNumber >= totalBlocks)
		return false; //out of bounds. It should never happen!!
	this->loadBlock(blockNumber);
	this->currentBlock = this->getCurrentBlock();
	int ovflwBlock = this->currentBlock->getOverflowedBlock();
	int availableBlock;
	if(!this->currentBlock->isEmpty() )
	{
		result = this->currentBlock->updateRecord(key, record);
		switch (result) {
			case UPDATED:
				this->hashBlockUsed = true;
				this->saveBlock();
				return true;
				break;
			case INSUFFICIENT_SPACE:
				// deleted from block, should insert in another block
				this->hashBlockUsed = true;
				this->overflowFile->loadBlock(ovflwBlock);
				availableBlock = this->getAvailableOverflowBlock(key, record);
				if(this->overflowFile->getCurrentBlock()->insertRecord(key, record))
				{
					this->ovflowBlockUsed = true;
					this->saveBlock();
					return true;
					break;
				}
				//Should NEVER enter here. it was removed from block 1,
				//and not inserted on any overflow block!!!
				this->saveBlock();
				return false;
				break;
			case NOT_FOUND:
				break; //search on overflowed block below
			default:
				break;
		}
	}
	if(ovflwBlock == -1)
		return false;//not found in hash and no overflow, stop.

	this->overflowFile->loadBlock(ovflwBlock);
	int foundInBlock = this->findInOverflowBlocks(key, &record, false);//the block is loaded
	if(foundInBlock == -1)
		return false;//key was not found

	result = this->overflowFile->getCurrentBlock()->updateRecord(key, record);
	switch(result){
	case UPDATED:
		this->ovflowBlockUsed = true;
		this->saveBlock();
		return true;
		break;
	case INSUFFICIENT_SPACE:
		// deleted from block, should insert in another block
		this->ovflowBlockUsed = true;
		this->saveBlock();
		ovflwBlock = this->getCurrentBlock()->getOverflowedBlock();
		this->overflowFile->loadBlock(ovflwBlock);
		availableBlock = this->getAvailableOverflowBlock(key, record);
		if(this->overflowFile->getCurrentBlock()->insertRecord(key, record))
		{
			this->ovflowBlockUsed = true;
			this->saveBlock();
			return true;
			break;
		}
		//Should NEVER enter here. it was removed from block 1,
		//and not inserted on any overflow block!!!
		return false;
		break;
	case NOT_FOUND:
		return false; //it shouldn't enter here
		break;
	default:
		break;
	}
	this->saveBlock();
	return false;
}

bool HashBlockFile::removeRecord(const char* key)
{
	//int blockNumber = this->fakeHashFunction(key);
	//to use on each Record Type :D
	int blockNumber = this->hashingFunction->hashingFunction(key, this->totalBlocks);

	if(blockNumber >= totalBlocks)
		return false; //out of bounds. It should never happen!!
	this->loadBlock(blockNumber);
	this->currentBlock = this->getCurrentBlock();
	if(!this->currentBlock->isEmpty() )
	{
		if(this->currentBlock->removeRecord(key) )
		{
			this->hashBlockUsed = true;
			this->saveBlock();
			return true;
		}
	}
	int ovflowBlock;
	int foundInBlock;
	VariableRecord* record;
	if((ovflowBlock = this->currentBlock->getOverflowedBlock()) != -1)
	{
		this->overflowFile->loadBlock(ovflowBlock);
		foundInBlock = this->findInOverflowBlocks(key, &record, true);//the block is loaded
		if(foundInBlock == -1)
			return false;
		if(this->overflowFile->getCurrentBlock()->removeRecord(key))
		{
			this->ovflowBlockUsed = true;
			this->saveBlock();

			//if empty, check above for pending registers
			if(this->overflowFile->getCurrentBlock()->isEmpty() &&
					this->overflowFile->getCurrentBlock()->getOverflowedBlock() == -1)
			{
				if(ovflowBlock == foundInBlock) //hash block becomes des-overflow
				{
					this->currentBlock->setNoOverflow();
					this->hashBlockUsed = true;
					this->saveBlock();
					return true;
				}
				this->overflowFile->loadBlock(ovflowBlock);
				while((ovflowBlock = this->overflowFile->getCurrentBlock()->getOverflowedBlock() )!= foundInBlock)
				{
					this->overflowFile->loadBlock(ovflowBlock);//overflow -1 should not appear!!
				}
				this->overflowFile->getCurrentBlock()->setNoOverflow();
				this->ovflowBlockUsed = true;
				this->saveBlock();
			}
			return true;
		}
	}
	//it doesn't get here
	return false;
}

int HashBlockFile::findInOverflowBlocks(const char* key, VariableRecord** record, bool getFlag)
{
	int ovflowBlock = this->currentBlock->getOverflowedBlock();
	SimpleVariableBlock* block = this->overflowFile->getCurrentBlock();
	VariableRecord* rec = new VariableRecord();
	while(block->findRecord(key, &rec) == -1)
	{
		ovflowBlock = block->getOverflowedBlock();
		if(ovflowBlock == -1)
		{
			delete rec;
			return ovflowBlock; //key was not found
		}
		this->overflowFile->loadBlock(ovflowBlock);
		block = this->overflowFile->getCurrentBlock();
	}
	if(getFlag)
		*record = rec;
	else
		delete rec;
	return ovflowBlock;
}

void HashBlockFile::loadBlock(int blockNumber)
{
	// It is supposed that the hash function gives a blockNumber
	// in a valid range for the file.
	this->currentBlockNumber = blockNumber;
    this->positionAtBlock(blockNumber);
    this->dataFile.read(this->currentBlock->getBytes(), this->blockSize);
    this->currentBlock->updateInformation();
}

void HashBlockFile::saveBlock()
{
	if(hashBlockUsed)
	{
		this->positionAtBlock(this->currentBlockNumber);
		this->dataFile.write(this->currentBlock->getBytes(), this->blockSize);
		hashBlockUsed = false;
	}
	if( ovflowBlockUsed )
	{
		//already position in block
		overflowFile->saveBlock();
		ovflowBlockUsed = false;
	}
}

HashBlockFile::~HashBlockFile()
{
	delete this->currentBlock;
	delete this->overflowFile;
	this->dataFile.close();
}

