/*
 * Record.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "Record.h"
#include <string.h>

Record::Record() : size(0)
{
	this->bytes = NULL;
}

int Record::getSize()
{
	return this->size;
}

void Record::setBytes(const char* value, int len)
{
	if (this->bytes != NULL)
	{
		// need to free memory
		delete[] this->bytes;
	}

	this->bytes = new char[len];
	memset(this->bytes, 0, len);
	memcpy(this->bytes, value, len);
	this->size = len;
}

char *Record::getBytes()
{
	return this->bytes;
}

Record& Record::operator =(const Record & other)
{
	if (this == &other)
	{
		return *this;
	}

	this->size = other.size;
	if (other.bytes != NULL)
	{
		this->setBytes(other.bytes, this->size);
	}
	else
	{
		this->bytes = NULL;
	}

	return *this;
}

Record::Record(Record& other)
{
	this->size = other.size;
	if (other.bytes != NULL)
	{
		this->setBytes(other.bytes, this->size);
	}
	else
	{
		this->bytes = NULL;
	}
}

Record::~Record()
{
	if (this->bytes != NULL)
	{
		// need to free memory
		delete[] this->bytes;
	}
}
