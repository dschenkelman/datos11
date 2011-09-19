/*
 * BlockFile.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "BlockFile.h"

using namespace std;

BlockFile::BlockFile(string & name, int bSize, int rSize, RecordMethods *methods, bool createNew):
		blockSize(bSize), currentBlockNumber(0)
{
	this->fileName = name;
	this->recordMethods = methods;
	this->currentBlock = new Block(this->blockSize, rSize, this->recordMethods);
	if (createNew)
	{
		this->dataFile.open(this->fileName.c_str(), ios::binary | ios::in | ios::out | ios::trunc);
	}
	else
	{
		this->dataFile.open(this->fileName.c_str(), ios::binary | ios::in | ios::out);
	}
}

void BlockFile::printContent()
{
	int blockNumber = 0;
	this->positionAtBlock(0);
	while(!this->isAtEOF())
	{
		this->loadBlock(blockNumber);
		this->currentBlock->printContent();
		blockNumber++;
	}
}

Block* BlockFile::getCurrentBlock()
{
	return this->currentBlock;
}

void BlockFile::positionAtBlock(int blockNumber)
{
    long position = blockNumber * this->blockSize;
    this->dataFile.clear();
    this->dataFile.seekg(position, ios::beg);
    this->dataFile.seekp(position, ios::beg);
}

bool BlockFile::isAtEOF()
{
	long position = this->dataFile.tellg();
	this->dataFile.seekg(0, ios::end);
	long size = this->dataFile.tellg();
	this->dataFile.seekg(position, ios::beg);
	return position == size;
}

void BlockFile::loadBlock(int blockNumber)
{
    this->currentBlockNumber = blockNumber;
	this->positionAtBlock(this->currentBlockNumber);
    if (!this->isAtEOF())
    {
    	this->dataFile.read(this->currentBlock->getBytes(), this->blockSize);
    }
    else
    {
    	memset(this->currentBlock->getBytes(), 0, this->blockSize);
    }
}

void BlockFile::saveBlock()
{
	this->positionAtBlock(this->currentBlockNumber);
	this->dataFile.write(this->currentBlock->getBytes(), this->blockSize);
}

BlockFile::~BlockFile()
{
	this->dataFile.close();
	delete this->currentBlock;
	delete this->recordMethods;
}
