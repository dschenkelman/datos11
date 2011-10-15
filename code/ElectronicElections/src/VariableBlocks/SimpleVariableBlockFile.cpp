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
	this->mapBlockNumber = 0;
	if (createNew)
	{
		this->dataFile.open(this->fileName.c_str(), ios::binary | ios::in | ios::out | ios::trunc);
		char initialValue[bSize * 2]; //to start with an empty block
		memset(initialValue, 0, bSize *2);
		int noOverflow = -1;// 4 bytes!
		memcpy(initialValue+bSize, &noOverflow, sizeof(int));
		memset(this->positionToDataBlocks, 0, bSize-4);
		this->blockAmount = 1;//relative block for each pagination.
		memcpy(initialValue, &blockAmount, sizeof(long));
		this->dataFile.seekp(0, ios::beg);
		this->dataFile.write(initialValue, this->blockSize * 2);
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
	//it searchs from one map block to other until it finds an empty place
	int mapBlock = 0;//reference to actual map block
	int i = 0; //block number para encontrar el libre
	int occupiedSize;
	int occupiedBlocks = this->blockAmount;
	this->dataFile.seekg(0, ios::end);
	long fileLength = this->dataFile.tellg();
	char positionData[this->blockSize-4];
	memcpy(positionData, this->positionToDataBlocks, this->blockSize -4);
	while((occupiedBlocks+1) == this->blockSize/sizeof(int))
	{
		if(i == occupiedBlocks)
		{
			mapBlock+= this->blockSize/sizeof(int);
			if(fileLength <= mapBlock * this->blockSize)
			{
				break;//creo nuevo bloque de mapa y devuelvo el siguiente
			}
			i = 0; //first data block!
			this->positionAtBlock(mapBlock);
			char bytes[this->blockSize];
			this->dataFile.read(bytes, this->blockSize);
			memcpy(&occupiedBlocks, bytes, 4);
			memcpy(positionData, bytes + 4, this->blockSize - 4);
			//this->loadMapBlock(mapBlock); //updates blockAmount & posToData
		}
		memcpy(&occupiedSize, (positionData + i*4), sizeof(int));
		if(occupiedSize == 0)
		{
			return mapBlock + i+1; //block is free
		}
		i++;
	}
	if((occupiedBlocks+1)  != this->blockSize/sizeof(int))
	{
		for(i=0; i < occupiedBlocks; i++)
		{
			memcpy(&occupiedSize, positionData + (i*4), 4);
			if(occupiedSize == 0) return mapBlock + i+1;
		}
		return (mapBlock + occupiedBlocks+1);
	}
	//nuevo map block
	char map[this->blockSize];
	occupiedBlocks = 1;
	memset(map, 0, this->blockSize);
	memcpy(map, &(occupiedBlocks), sizeof(int));
	this->dataFile.seekp(mapBlock * this->blockSize, ios::beg);
	this->dataFile.write(map, this->blockSize);
	memset(positionData, 0, this->blockSize-4);
	return mapBlock + 1; //all occupied. returns a new one.
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
		if(blockNumber % (this->blockSize/sizeof(int)) == 0)
		{
			blockNumber++;//doesnt print the map block
		}
	}
	//this->updateBlockAmount();
}

bool SimpleVariableBlockFile::internalInsertRecord(const char* key,
		const char* recordBytes, short size, bool force)
{
	int blockNumber = 1;
	int blockToInsert = -1;

	this->positionAtBlock(1);
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
		this->positionAtBlock(blockNumber);
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
	VariableRecord* r = NULL;
	while(!this->isAtEOF())
	{
		this->loadBlock(blockNumber);

		if (this->currentBlock->findRecord(key, &r) >= 0)
		{
			*rec = r;
			return true;
		}
		blockNumber++;
	}
	if (r != NULL)
	{
		delete r;
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

void SimpleVariableBlockFile::loadMapBlock(int mapBlock)
{
	if(this->mapBlockNumber != mapBlock)
	{
		this->mapBlockNumber = mapBlock;
		this->positionAtBlock(this->mapBlockNumber);
		char bytes[this->blockSize];
		this->dataFile.read(bytes, this->blockSize);
		memcpy(&this->blockAmount, bytes, 4);
		memcpy(this->positionToDataBlocks, bytes + 4, this->blockSize - 4);
	}
}

void SimpleVariableBlockFile::loadRespectiveMapBlock(int blockNumber)
{
	//encuentra el mapa respectivo y lo carga.
	int blocksInMap = this->blockSize/ sizeof(int);
	int mapBlock = (blockNumber/ blocksInMap) * blocksInMap;
	this->loadMapBlock(mapBlock);
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
    this->loadRespectiveMapBlock(blockNumber);
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
	char bytes[this->blockSize];
	memcpy(bytes, this->currentBlock->getBytes(), this->blockSize);
	int overflwBlock = this->getCurrentBlock()->getOverflowedBlock();
	memcpy(bytes, &overflwBlock, sizeof(int));//puntero 4 bytes
	this->dataFile.write(bytes, this->blockSize);

	//writing occupied size of all blocks in first block
	int occupiedSpace = this->blockSize - this->currentBlock->getFreeSpace();
	int blockOffset = (this->currentBlockNumber % (this->blockSize/sizeof(int)) ) -1;
	memcpy(this->positionToDataBlocks + (blockOffset* 4), &occupiedSpace, 4);
	this->updateBlockAmount();
	this->dataFile.seekp(this->mapBlockNumber* this->blockSize, ios::beg);
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
	int totalBlocks = size / this->blockSize - 1; //less 1 for the first block that it doesn't count
	if( (this->mapBlockNumber + this->blockSize/sizeof(int) -1) <= totalBlocks)
	{
		this->blockAmount = this->blockSize/sizeof(int) -1;
	}
	else
	{
	this->blockAmount = totalBlocks - this->mapBlockNumber;
	}
}
SimpleVariableBlockFile::~SimpleVariableBlockFile()
{
	this->dataFile.close();
	delete this->currentBlock;
	delete[] this->positionToDataBlocks;
}
