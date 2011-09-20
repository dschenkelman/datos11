/*
 * TreeBlock.cpp
 *
 *  Created on: Sep 20, 2011
 *      Author: damian
 */

#include "TreeBlock.h"
#include <string.h>

TreeBlock::TreeBlock(int size, int recordSize, RecordMethods* methods)
	:Block(size, recordSize, methods, BLOCK_POINTER_SIZE)
{
}

void TreeBlock::setNextBlock(int blockNumber)
{
	memcpy(this->bytes + (this->maxSize - BLOCK_POINTER_SIZE),
			&blockNumber, BLOCK_POINTER_SIZE);
}

TreeBlock::~TreeBlock()
{
}
