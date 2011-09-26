/*
 * BaseVariableBlock.cpp
 *
 *  Created on: Sep 25, 2011
 *      Author: damian
 */

#include "BaseVariableBlock.h"
#include <cstddef>
#include <string.h>

BaseVariableBlock::BaseVariableBlock(int size, int pos,
		int rOffset, RecordMethods* methods) :
		maxSize(size), position(pos), recordsOffset(rOffset), recordMethods(methods)
{
	this->bytes = new char[this->maxSize];
	memset(this->bytes, 0, this->maxSize);
	this->freeSpace = this->maxSize;
}

char* BaseVariableBlock::getBytes()
{
	return this->bytes;
}

int BaseVariableBlock::getFreeSpace()
{
	return this->freeSpace;
}

int BaseVariableBlock::findRecord(const char* key, VariableRecord** rec)
{
	this->position = this->recordsOffset;
	VariableRecord* record = new VariableRecord();
	int foundPosition = this->position;
	while(this->getNextRecord(record) != NULL)
	{
		if (this->recordMethods->compare(key,
				record->getBytes(), record->getSize()) == 0)
		{
			*rec = record;
			return foundPosition;
		}
		delete record;
		record = new VariableRecord();
		foundPosition = this->position;
	}

	delete record;
	return -1;
}


int BaseVariableBlock::getSize()
{
	return this->maxSize;
}

int BaseVariableBlock::getOccupiedSize()
{
	return this->maxSize - this->freeSpace;
}

void BaseVariableBlock::positionAtBegin()
{
	this->position = this->recordsOffset;
}

BaseVariableBlock::~BaseVariableBlock()
{
	delete[] this->bytes;
}
