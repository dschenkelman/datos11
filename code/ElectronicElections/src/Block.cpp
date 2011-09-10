/*
 * Block.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "Block.h"
#include <string.h>

Block::Block(int size) : maxSize(size)
{
	this->bytes = new char[this->maxSize];
	memset(this->bytes, 0, this->maxSize);
}

char* Block::getBytes()
{
	return this->bytes;
}

Block::~Block()
{
	delete[] this->bytes;
}
