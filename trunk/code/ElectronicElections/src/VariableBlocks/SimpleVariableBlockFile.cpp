/*
 * RLVBlockFile.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "SimpleVariableBlockFile.h"

using namespace std;

SimpleVariableBlockFile::SimpleVariableBlockFile
(string& name, int bSize, RecordMethods* methods, bool createNew):
BaseVariableBlockFile(name, bSize, methods)
{
	//first block in file:
	//long for blocksize, long for blockamount, all longs for occupied space in each block.
	this->fileName = name;
	this->recordMethods = methods;
	this->positionToDataBlocks = new char[bSize-4];
	this->currentBlock = new SimpleVariableBlock(this->blockSize, this->recordMethods);
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

int SimpleVariableBlockFile::getFirstFreeEmptyBlock()
{
	//it returns -1 if there are no empty blocks
	int occupiedSize;
	for(int i=0; i < this->blockAmount; i++)
	{
		memcpy(&occupiedSize, this->positionToDataBlocks + i*4, 4);
		if(occupiedSize == 0) return i+1;

	}
	return -1;
}

void SimpleVariableBlockFile::printContent()
{
	int blockNumber = 1;
	this->positionAtBlock(0);
	while(!this->isAtEOF())
	{
		this->loadBlock(blockNumber);
		this->currentBlock->printContent();
		blockNumber++;
	}
	this->updateBlockAmount();
}

bool SimpleVariableBlockFile::internalInsertRecord(const char* key,
		const char* recordBytes, short size, bool force)
{
	int blockNumber = 1;
	int blockToInsert = -1;

	this->positionAtBlock(0);
	while(!this->isAtEOF())
	{
		this->loadBlock(blockNumber);
		if(!force)
		{
			VariableRecord* r = NULL;
			if (this->currentBlock->findRecord(key, &r) >= 0)
			{
				if (r != NULL)
				{
					delete r;
				}

				return false;
			}

			if (r != NULL)
			{
				delete r;
			}
		}

		if (this->currentBlock->canInsertRecord(size) && blockToInsert == -1)
		{
			// first available place to put record
			blockToInsert = blockNumber;
		}
		blockNumber++;
	}

	if (blockToInsert != -1)
	{
		this->loadBlock(blockToInsert);
	}
	else
	{
		// file is full, new block is required
		this->loadBlock(blockNumber);
		this->currentBlock->clear();
	}

	VariableRecord* record = new VariableRecord();
	record->setBytes(recordBytes, size);
	bool result = this->currentBlock->insertRecord(key, record);
	this->saveBlock();
	delete record;

	return result;
}

bool SimpleVariableBlockFile::insertRecord(const char* key, const char* recordBytes, short size)
{
	return this->internalInsertRecord(key, recordBytes, size, false);
}

bool SimpleVariableBlockFile::updateRecord(const char *key, const char *recordBytes, short size)
{
	int blockNumber = 1;

	this->positionAtBlock(0);
	VariableRecord* r = new VariableRecord();
	r->setBytes(recordBytes, size);

	while(!this->isAtEOF())
	{
		this->loadBlock(blockNumber);

		UpdateResult result = this->currentBlock->updateRecord(key, r);
		switch (result) {
			case UPDATED:
				delete r;
				this->saveBlock();
				return true;
				break;
			case INSUFFICIENT_SPACE:
				// deleted from block, should insert in another block
				this->internalInsertRecord(key, recordBytes, size, true);
			case NOT_FOUND:
			default:
				break;
		}

		blockNumber++;
	}

	delete r;
	return false;
}

bool SimpleVariableBlockFile::removeRecord(const char* key)
{
	int blockNumber = 1;

	this->positionAtBlock(0);
	while(!this->isAtEOF())
	{
		this->loadBlock(blockNumber);
		VariableRecord* r = NULL;
		if (this->currentBlock->removeRecord(key) )
		{
			delete r;
			this->saveBlock();
			return true;
		}
		if (r != NULL)
		{
			delete r;
		}
		blockNumber++;
	}
	return false;
}

bool SimpleVariableBlockFile::getRecord(const char *key, VariableRecord** rec)
{
	int blockNumber = 1;

	this->positionAtBlock(0);
	while(!this->isAtEOF())
	{
		this->loadBlock(blockNumber);
		if (this->currentBlock->findRecord(key, rec) >= 0)
		{
			return true;
		}

		blockNumber++;
	}

	return false;
}

void SimpleVariableBlockFile::positionAtBlock(int blockNumber)
{
    long position = blockNumber * this->blockSize;
    this->dataFile.clear();
    this->dataFile.seekg(position, ios::beg);
    this->dataFile.seekp(position, ios::beg);
}

bool SimpleVariableBlockFile::isAtEOF()
{
	long position = this->dataFile.tellg();
	this->dataFile.seekg(0, ios::end);
	long size = this->dataFile.tellg();
	this->dataFile.seekg(position, ios::beg);
	return position == size;
}

void SimpleVariableBlockFile::loadBlock(int blockNumber)
{
	this->currentBlockNumber = blockNumber;
    this->positionAtBlock(blockNumber);

    if (!this->isAtEOF())
    {
    	this->dataFile.read(this->currentBlock->getBytes(), this->blockSize);
    }
    else
    {
    	memset(this->currentBlock->getBytes(), 0, this->blockSize);
    }

    this->currentBlock->updateInformation();
    this->positionAtBlock(blockNumber);
}

SimpleVariableBlock* SimpleVariableBlockFile::getCurrentBlock()
{
	return this->currentBlock;
}

void SimpleVariableBlockFile::saveBlock()
{

	this->positionAtBlock(this->currentBlockNumber);
	int occupiedSpace = this->blockSize - this->currentBlock->getFreeSpace();
	this->dataFile.write(this->currentBlock->getBytes(), this->blockSize);
	//writing occupied size of all blocks in first block
	memcpy(this->positionToDataBlocks + (this->currentBlockNumber-1) * 4,
				&occupiedSpace, 4);
	this->updateBlockAmount();
	this->dataFile.seekp(0, ios::beg);
	char* dataBlocks = new char[this->blockSize];
	memset(dataBlocks, 0, this->blockSize);
	memcpy(dataBlocks, &this->blockAmount, 4);
	memcpy(dataBlocks + 4, this->positionToDataBlocks, this->blockSize-4);
	this->dataFile.write(dataBlocks, this->blockSize);
	delete [] dataBlocks;
}
void SimpleVariableBlockFile::updateBlockAmount()
{
	this->dataFile.seekg(0, ios::end);
	long size = this->dataFile.tellg();
	this->blockAmount = size / this->blockSize - 1; //less 1 for the first block that it doesn't count
}

SimpleVariableBlockFile::~SimpleVariableBlockFile()
{
	//writing occupied size of all blocks in first block
	this->dataFile.seekp(0, ios::beg);
	char* dataBlocks = new char[this->blockSize];
	memset(dataBlocks, 0, this->blockSize);
	memcpy(dataBlocks, &this->blockAmount, 4);
	memcpy(dataBlocks + 4, this->positionToDataBlocks, this->blockSize-4);
	this->dataFile.write(dataBlocks, this->blockSize);
	delete [] dataBlocks;
	this->dataFile.close();
	delete this->currentBlock;
	delete this->recordMethods;
	delete this->positionToDataBlocks;
}
