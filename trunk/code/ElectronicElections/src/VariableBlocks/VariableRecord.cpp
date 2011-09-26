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
