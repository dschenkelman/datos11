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
	//const int ovflwFileLen = strlen(fileName.c_str()) + 7;
	//char ovflwFile[ovflwFileLen];
	//strcat(ovflwFile, fileName.c_str());
	//strcat(ovflwFile, ovflw);
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

int HashBlockFile::hashFunction(char* key)
{
	// This will change
	//int hash = atoi(key) % blockAmount;
	return 2;
}

void HashBlockFile::printContent()
{
	int blockNumber = 0;
	this->positionAtBlock(0);
	while(!this->isAtEOF())
	{
		this->loadBlock(blockNumber);
		if(!this->currentBlock->isEmpty() )
		{
			this->currentBlock->printContent();
		}
		blockNumber++;
	}
}

bool HashBlockFile::internalInsertRecord(const char* key,
		const char* recordBytes, short size, bool force)
{
	return true;
}

bool HashBlockFile::insertRecord(const char* key, const char* recordBytes, short size)
{
	int blockNumber = 2;
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
			if( record!= NULL) delete record;
			return false;
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

