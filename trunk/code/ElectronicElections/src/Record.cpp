/*
 * Record.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "Record.h"
#include <string.h>

Record::Record()
{
	this->bytes = NULL;
}

int Record::getSize()
{
	return this->size;
}

void Record::setBytes(char* value, int len)
{
	if (this->bytes != NULL)
	{
		// need to free memory
		delete[] this->bytes;
	}

	this->bytes = new char[len];
	memset(this->bytes, 0, len);
	memcpy(this->bytes, value, len);
}

char *Record::getBytes()
{
	return this->bytes;
}

Record::~Record()
{
	if (this->bytes != NULL)
	{
		// need to free memory
		delete[] this->bytes;
	}
}
