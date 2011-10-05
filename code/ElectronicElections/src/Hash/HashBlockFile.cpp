/*
 * HashBlockFile.cpp
 *
 *  Created on: Sep 22, 2011
 *      Author: Gaby+JM
 */

#include "HashBlockFile.h"

using namespace std;

HashBlockFile::HashBlockFile(std::string fileName, int bSize, RecordMethods *methods, int blockAmount, bool createNew)
: BaseVariableBlockFile (fileName, bSize, methods)
{
	this->ovflowBlockUsed = false;
	this->hashBlockUsed = false;
	this->totalBlocks = blockAmount;
	this->currentBlock = new HashBlock(bSize, methods);
	if( createNew)
	{
		this->initializefile();
	}
	else
	{
		this->dataFile.open(this->fileName.c_str(), ios::binary | ios::in | ios::out);
	}
	string ovflw = "ovflow";
	this->overflowFile = new SimpleVariableBlockFile(ovflw, bSize, methods, createNew);
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

int HashBlockFile::hashFunction(const char* key)
{
	// This will change
	//int hash = atoi(key) % blockAmount;
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
		}
		if( (ovflwBlock = this->currentBlock->getOverflowedBlock()) != -1)
		{
			this->overflowFile->loadBlock(ovflwBlock);
			this->overflowFile->getCurrentBlock()->printContent();
		}
		blockNumber++;
	}
}

int HashBlockFile::getAvailableOverflowBlock(const char* key, VariableRecord* record)
{
	SimpleVariableBlock* block = this->overflowFile->getCurrentBlock();
	int freeSpace = block->getFreeSpace();
	int ovflwBlock;
	bool foundRecord = false;
	while(freeSpace < record->getSize()+2)
	{
		if(block->findRecord(key, &record))
		{
			foundRecord = true;
			break;
		}
		ovflwBlock = block->getOverflowedBlock();
		if(ovflwBlock == -1)
			break;
		this->overflowFile->loadBlock(ovflwBlock);
		block = this->overflowFile->getCurrentBlock();
		freeSpace = block->getFreeSpace();
	}
	if(foundRecord)
		return -1;//error number. value found!
	if( ovflwBlock == -1)
	{
		int freeBlock = this->overflowFile->getFirstFreeEmptyBlock();
		this->overflowFile->getCurrentBlock()->becomesOverflow(freeBlock);
		this->ovflowBlockUsed = true;
		this->saveBlock();
		return ovflwBlock;
	}
	//enough space in block. insert
	return ovflwBlock;
}

bool HashBlockFile::insertRecord(const char* key, const char* recordBytes, short size)
{
	int blockNumber = this->hashFunction(key);
	if(blockNumber >= totalBlocks)
		return false; //out of bounds. It should never happen!!

	int ovflowBlock;
	this->loadBlock(blockNumber);
	this->currentBlock = this->getCurrentBlock();
	VariableRecord* record = new VariableRecord();
	record->setBytes(recordBytes, size);
	if(this->currentBlock->findRecord(key, &record) >= 0)
	{
		//ERROR, KEY ALREADY INSERTED
		if( record!= NULL) delete record;
		return false;
	}
	if(this->currentBlock->canInsertRecord(size) )
	{
		if( (ovflowBlock = this->currentBlock->getOverflowedBlock()) != -1)
		{
			this->overflowFile->loadBlock(ovflowBlock);
			if(this->overflowFile->getRecord(key, &record) )
			{
				if( record!= NULL) delete record;
				return false;
			}
		}
		this->hashBlockUsed = true;
		this->currentBlock->insertRecord(key, record);
		this->saveBlock();
		if( record!= NULL) delete record;
		return true;
	}
	//no space in block, overflow!
	if((ovflowBlock = this->currentBlock->getOverflowedBlock()) != -1)
	{
		this->overflowFile->loadBlock(ovflowBlock);
		if(!this->overflowFile->insertRecord(key, recordBytes, size) )
		{
			//overflow block is full. insert in another
			if((ovflowBlock = this->getAvailableOverflowBlock(key, record)) == -1)
				return false; //key was found
			this->overflowFile->loadBlock(ovflowBlock);
			this->overflowFile->insertRecord(key, recordBytes, size);
		}
		this->ovflowBlockUsed = true;
		this->saveBlock();
		delete record;
		return true;
	}
	//it wasn't overflowed. now it is
	this->hashBlockUsed = true;
	this->ovflowBlockUsed = true;
	ovflowBlock = this->overflowFile->getFirstFreeEmptyBlock();
	this->currentBlock->becomesOverflow(ovflowBlock);
	this->overflowFile->loadBlock(ovflowBlock);
	this->overflowFile->insertRecord(key, recordBytes, size);
	this->saveBlock();
	delete record;
	return true;
}

bool HashBlockFile::getRecord(const char* key, VariableRecord** record)
{
	int blockNumber = this->hashFunction(key);
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
	int blockNumber = this->hashFunction(key);
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
				if(this->overflowFile->insertRecord(key, record->getBytes(),record->getSize()))
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
		if(this->overflowFile->insertRecord(key, record->getBytes(),record->getSize()))
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
	int blockNumber = this->hashFunction(key);
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
			if(this->overflowFile->getCurrentBlock()->isEmpty())
			{
				this->currentBlock->setNoOverflow();
				this->hashBlockUsed = true;
			}
			this->saveBlock();
			return true;
		}
	}
	//it doesn't get here
	return false;
}

int HashBlockFile::findInOverflowBlocks(const char* key, VariableRecord** record, bool getFlag)
{
	int ovflowBlock;
	SimpleVariableBlock* block = this->overflowFile->getCurrentBlock();
	VariableRecord* rec = new VariableRecord();
	while(block->findRecord(key, &rec) == -1)
	{
		ovflowBlock = block->getOverflowedBlock();
		if(ovflowBlock == -1)
			return ovflowBlock; //key was not found
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
		int ovflwBlock = this->currentBlock->getOverflowedBlock();
		overflowFile->positionAtBlock(ovflwBlock);
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

