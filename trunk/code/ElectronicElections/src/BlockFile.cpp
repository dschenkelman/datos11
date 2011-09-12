/*
 * BlockFile.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "BlockFile.h"

using namespace std;

BlockFile::BlockFile(string& name, int bSize, RecordComparer* comparer, bool createNew):blockSize(bSize)
{
	this->fileName = name;
	this->recordComparer = comparer;
	this->currentBlock = new Block(this->blockSize, this->recordComparer);
	if (createNew)
	{
		this->dataFile.open(this->fileName.c_str(), ios::binary | ios::in | ios::out | ios::trunc);
		char initialValue[512];
		memset(initialValue, 0, 512);
		this->dataFile.write(initialValue, 512);
	}
	else
	{
		this->dataFile.open(this->fileName.c_str(), ios::binary | ios::in | ios::out);
	}
}

bool BlockFile::insertRecord(char* key, char* recordBytes, int size)
{
	int blockNumber = 1;
	int blockToInsert = -1;

	this->positionAtBlock(0);
	while(!this->dataFile.eof())
	{
		this->loadBlock(blockNumber);
		this->currentBlock->updateInformation();
		if (this->currentBlock->findRecord(key))
		{
			return false;
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
		this->currentBlock->updateInformation();
	}
	else
	{
		// file is full, new block is required
		this->positionAtBlock(blockNumber);
		this->currentBlock->clear();
	}

	Record* record = new Record();
	record->setBytes(recordBytes, size);
	this->currentBlock->insertRecord(record);
	this->saveBlock();
	delete record;

	return true;
}

void BlockFile::positionAtBlock(int blockNumber)
{
    long position = blockNumber * this->blockSize;
    this->dataFile.seekg(position, ios::beg);
    this->dataFile.seekp(position, ios::beg);
}

void BlockFile::loadBlock(int blockNumber)
{
    this->positionAtBlock(blockNumber);
	this->dataFile.read(this->currentBlock->getBytes(), this->blockSize);
}

void BlockFile::saveBlock()
{
	this->dataFile.write(this->currentBlock->getBytes(), this->blockSize);
}

BlockFile::~BlockFile() {
	this->dataFile.close();
	delete this->currentBlock;
	delete this->recordComparer;
}
