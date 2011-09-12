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

void BlockFile::loadBlock(int blockNumber)
{
	long position = blockNumber * this->blockSize;
	this->dataFile.seekg(position, ios::beg);
	this->dataFile.seekp(position, ios::end);
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
