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
	this->free = true;
	this->erased = false;
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

bool Record::isFree()
{
	return this->free;
}

/* this method tells if the record is erased and consequently free.
 * It is worthless if the record is erased but occupied.
 */
bool Record::isErased()
{
	return ( this->free && this->erased );
}

void Record::setRecordStatus(bool freeRecord, bool erasedRecord)
{
	this->free = freeRecord;
	this->erased = erasedRecord;
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
