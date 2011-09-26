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

TreeBlock::~TreeBlock()
{
}
