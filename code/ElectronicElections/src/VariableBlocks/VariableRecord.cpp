/*
 * VariableRecord.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "VariableRecord.h"
#include <string.h>

VariableRecord::VariableRecord() : size(0)
{
	this->bytes = NULL;
}

VariableRecord::VariableRecord(const char* value, short len)
{
	this->bytes = NULL;
	this->setBytes(value,len);
}

VariableRecord::VariableRecord(VariableRecord& other)
{
	if (other.size != 0)
	{
		this->bytes = new char[other.size];
		memcpy(this->bytes, other.bytes, other.size);
	}

	this->size = other.size;
}

VariableRecord& VariableRecord::operator =(const VariableRecord& other)
{
	if (this == &other)
	{
		return *this;
	}

	if (this->size != 0)
	{
		delete[] this->bytes;
	}

	if (other.size != 0)
	{
		this->bytes = new char[other.size];
		memcpy(this->bytes, other.bytes, other.size);
	}

	this->size = other.size;

	return *this;
}

short VariableRecord::getSize()
{
	return this->size;
}

void VariableRecord::setBytes(const char* value, short len)
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

char *VariableRecord::getBytes()
{
	return this->bytes;
}

VariableRecord::~VariableRecord()
{
	if (this->bytes != NULL)
	{
		// need to free memory
		delete[] this->bytes;
	}
}
