/*
 * TreeBlockFile.cpp
 *
 *  Created on: Sep 20, 2011
 *      Author: damian
 */

#include "TreeBlockFile.h"
#include <iostream>
#include <string.h>
#include "SequenceTreeBlock.h"
#include "IndexTreeBlock.h"

using namespace std;

TreeBlockFile::TreeBlockFile(std::string& fileName, int bSize,
		RecordMethods *methods, bool createNew) :
		BaseVariableBlockFile(fileName, bSize, methods)
{
	this->currentBlock = NULL;
	if (createNew)
	{
		this->dataFile.open(this->fileName.c_str(), ios::binary | ios::in | ios::out | ios::trunc);
	}
	else
	{
		this->dataFile.open(this->fileName.c_str(), ios::binary | ios::in | ios::out);
	}
}

TreeBlock* TreeBlockFile::getCurrentBlock()
{
	return this->currentBlock;
}

void TreeBlockFile::printContent()
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

void TreeBlockFile::loadBlock(int blockNumber)
{
	char buffer[this->blockSize];
	this->currentBlockNumber = blockNumber;
	this->positionAtBlock(this->currentBlockNumber);
	if (!this->isAtEOF())
	{
		this->dataFile.read(buffer, this->blockSize);
	}
	else
	{
		this->getCurrentBlock()->clear();
	}

	short level;
	memcpy(&level, buffer, TreeBlock::LEVEL_SIZE);

	delete this->currentBlock;
	if (level == 0)
	{
		// assign SequenceTreeBlock;
		this->currentBlock = new SequenceTreeBlock(this->blockSize, this->recordMethods);
	}
	else
	{
		// assign IndexTreeBlock;
		this->currentBlock = new IndexTreeBlock(this->blockSize, this->recordMethods);
	}

	if (!this->isAtEOF())
	{
		memcpy(this->currentBlock->getBytes(), buffer, this->blockSize);
		this->currentBlock->updateInformation();
	}
}

void TreeBlockFile::saveBlock()
{
	this->positionAtBlock(this->currentBlockNumber);
	//this->dataFile.write(this->getCurrentBlock()->getBytes(), this->blockSize);
}

TreeBlockFile::~TreeBlockFile()
{
	delete this->currentBlock;
	this->dataFile.close();
}
