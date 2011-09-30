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
		BaseVariableBlockFile(fileName, bSize, methods),
		freeBlockManager(fileName.replace(fileName.length() - 4, 4, ".adm"), createNew),
		isLeaf(false)
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
	if  (this->blockStack.empty())
	{
		return NULL;
	}

	return this->blockStack.top();
}

void TreeBlockFile::printContent()
{
	int blockNumber = 0;
	this->positionAtBlock(0);
	while(!this->isAtEOF())
	{
		this->loadBlock(blockNumber);
		this->pushBlock();
		this->currentBlock->printContent();
		this->popBlock();
		blockNumber++;
	}
}

void TreeBlockFile::loadBlock(int blockNumber)
{
	char buffer[this->blockSize];
	this->currentBlockNumber = blockNumber;
	this->positionAtBlock(this->currentBlockNumber);
	short level = 0;
	if (!this->isAtEOF())
	{
		this->dataFile.read(buffer, this->blockSize);
		memcpy(&level, buffer, TreeBlock::LEVEL_SIZE);
	}
	else
	{
		if (this->getCurrentBlock() != NULL)
		{
			this->getCurrentBlock()->clear();
		}
	}

	if (level == 0)
	{
		// assign SequenceTreeBlock;
		this->currentBlock = new SequenceTreeBlock(this->blockSize, this->recordMethods);
		this->isLeaf = true;
	}
	else
	{
		// assign IndexTreeBlock;
		this->currentBlock = new IndexTreeBlock(this->blockSize, this->recordMethods);
		this->isLeaf = false;
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
	this->dataFile.write(this->getCurrentBlock()->getBytes(), this->blockSize);
}

bool TreeBlockFile::isCurrentLeaf()
{
	return isLeaf;
}

FreeBlockManager& TreeBlockFile::getFreeBlockManager()
{
	return this->freeBlockManager;
}

void TreeBlockFile::popBlock()
{
	delete this->currentBlock;
	this->currentBlock = this->blockStack.top();
	this->blockStack.pop();
}

void TreeBlockFile::pushBlock()
{
	this->blockStack.push(this->currentBlock);
}

TreeBlockFile::~TreeBlockFile()
{
	delete this->currentBlock;
	this->dataFile.close();
}
