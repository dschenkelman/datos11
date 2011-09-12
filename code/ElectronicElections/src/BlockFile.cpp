/*
 * BlockFile.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "BlockFile.h"

using namespace std;

BlockFile::BlockFile(string fileName, int bSize, RecordComparer* comparer):blockSize(bSize)
{
	this->recordComparer = comparer;
	this->currentBlock = new Block(this->blockSize, this->recordComparer);
	this->dataFile.open(fileName.c_str(), ios::binary | ios::in | ios::out);
}

bool BlockFile::insertRecord(char* key, Record *record)
{
	int blockNumber = 1;
	int blockToInsert = -1;

	while(!this->dataFile.eof())
	{
		this->loadBlock(blockNumber);
		this->currentBlock->updateInformation();
		if (this->currentBlock->findRecord(key))
		{
			return false;
		}

		if (this->currentBlock->canInsertRecord(record) && blockToInsert == -1)
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

	this->currentBlock->insertRecord(record);
			this->saveBlock();

	return true;
}

void BlockFile::positionAtBlock(int blockNumber)
{
    long position = blockNumber * this->blockSize;
    this->dataFile.seekg(position, ios::beg);
    this->dataFile.seekp(position, ios::end);
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
