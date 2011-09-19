/*
 * Record.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "Record.h"
#include <string.h>

Record::Record(int s) : size(s), isEmpty(true), wasDeleted(false)
{
	this->bytes = new char[this->size];
	memset(this->bytes, 0, this->size);
}

int Record::getSize()
{
	return this->size;
}

void Record::setBytes(const char* value)
{
	memcpy(this->bytes, value, this->size);
}

char *Record::getBytes()
{
	return this->bytes;
}

bool Record::getIsEmpty()
{
	return this->isEmpty;
}

bool Record::getWasDeleted()
{
	return this->wasDeleted;
}

void Record::setWasDeleted(bool value)
{
	this->wasDeleted = value;
}

void Record::setIsEmpty(bool value)
{
	this->isEmpty = value;
}

Record::~Record()
{
	// need to free memory
	delete[] this->bytes;
}
