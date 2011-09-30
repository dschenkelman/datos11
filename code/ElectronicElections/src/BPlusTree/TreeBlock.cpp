/*
 * TreeBlock.cpp
 *
 *  Created on: Sep 25, 2011
 *      Author: damian
 */

#include "TreeBlock.h"
#include <string.h>

TreeBlock::TreeBlock(int size, int pos,
		int rOffset, RecordMethods* methods)
:BaseVariableBlock(size, pos, rOffset, methods)
{
}

void TreeBlock::setLevel(short value)
{
	memcpy(this->bytes, &value, LEVEL_SIZE);
}

short TreeBlock::getLevel()
{
	short value;
	memcpy(&value, this->bytes, LEVEL_SIZE);
	return value;
}

VariableRecord* TreeBlock::getNextRecord(VariableRecord *r)
{
	if (!this->hasNextRecord())
	{
		return NULL;
	}

	short recordSize;
	memcpy(&recordSize, this->bytes + this->position, Constants::RECORD_HEADER_SIZE);
	this->position += Constants::RECORD_HEADER_SIZE;
	r->setBytes(this->bytes + this->position, recordSize);
	this->position += recordSize;

	return r;
}


void TreeBlock::updateFreeSpace(int space)
{
    this->freeSpace = space;
    memcpy(this->bytes + TreeBlock::LEVEL_SIZE, &this->freeSpace, TreeBlock::FREE_SPACE_SIZE);
}

void TreeBlock::positionAtBegin()
{
	this->position = this->recordsOffset;
}

TreeBlock::~TreeBlock()
{
}
