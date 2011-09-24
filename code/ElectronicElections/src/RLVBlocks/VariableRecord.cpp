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

int VariableRecord::getSize()
{
	return this->size;
}

void VariableRecord::setBytes(const char* value, int len)
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

VariableRecord& VariableRecord::operator =(const VariableRecord & other)
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

VariableRecord::VariableRecord(VariableRecord& other)
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

VariableRecord::~VariableRecord()
{
	if (this->bytes != NULL)
	{
		// need to free memory
		delete[] this->bytes;
	}
}
