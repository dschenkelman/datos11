/*
 * BlockFile.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "BlockFile.h"

using namespace std;

BlockFile::BlockFile(string& name, int bSize, RecordMethods* methods, bool createNew):blockSize(bSize)
{
	this->fileName = name;
	this->recordMethods = methods;
	this->currentBlock = new Block(this->blockSize, this->recordMethods);
	if (createNew)
	{
		this->dataFile.open(this->fileName.c_str(), ios::binary | ios::in | ios::out | ios::trunc);
		char initialValue[bSize];
		memset(initialValue, 0, bSize);
		this->dataFile.write(initialValue, bSize);
	}
	else
	{
		this->dataFile.open(this->fileName.c_str(), ios::binary | ios::in | ios::out);
	}
}
void BlockFile::printContent()
{
	int blockNumber = 1;
	this->positionAtBlock(0);
	while(!this->isAtEOF())
	{
		this->loadBlock(blockNumber);
		this->currentBlock->printContent();
		blockNumber++;
	}
}

bool BlockFile::internalInsertRecord(const char* key,
		const char* recordBytes, int size, bool force)
{
	int blockNumber = 1;
	int blockToInsert = -1;

	this->positionAtBlock(0);
	while(!this->isAtEOF())
	{
		this->loadBlock(blockNumber);
		if(!force)
		{
			Record* r = NULL;
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
		this->positionAtBlock(blockNumber);
		this->currentBlock->clear();
	}

	Record* record = new Record();
	record->setBytes(recordBytes, size);
	bool result = this->currentBlock->insertRecord(record);
	this->saveBlock();
	delete record;

	return result;
}

bool BlockFile::insertRecord(const char* key, const char* recordBytes, int size)
{
	return this->internalInsertRecord(key, recordBytes, size, false);
}

bool BlockFile::updateRecord(const char *key, const char *recordBytes, int size)
{
	int blockNumber = 1;

	this->positionAtBlock(0);
	Record* r = new Record();
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

bool BlockFile::removeRecord(const char* key)
{
	int blockNumber = 1;

	this->positionAtBlock(0);
	while(!this->isAtEOF())
	{
		this->loadBlock(blockNumber);
		Record* r = NULL;
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

void BlockFile::saveBlock()
{
	this->dataFile.write(this->currentBlock->getBytes(), this->blockSize);
	this->currentBlock->updateInformation();
}

BlockFile::~BlockFile() {
	this->dataFile.close();
	delete this->currentBlock;
	delete this->recordMethods;
}
