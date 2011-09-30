/*
 * District.cpp
 *
 *  Created on: 27/09/2011
 *      Author: gonzalo
 */

#include "District.h"
#include "../VariableBlocks/Constants.h"
#include <cstring>

District::District(std::string name)
{
	this->name = name;
	this->bytes = NULL;
}

std::string District::getName()
{
	return this->name;
}

int District::getKeySize()
{
	return this->getSize();
}

int District::getSize()
{
	return this->name.size() + Constants::RECORD_HEADER_SIZE +
			Constants::FIELD_HEADER_SIZE + 1;
}

char* District::getKey()
{
	return this->getBytes();
}

char* District::getBytes()
{
	short size = this->getSize();

	if(this->bytes != NULL)
	{
		delete this->bytes;
	}

	this->bytes = new char[size];

	size -= Constants::RECORD_HEADER_SIZE;

	int i = 0;

	memcpy(this->bytes, &size, Constants::RECORD_HEADER_SIZE); i += Constants::RECORD_HEADER_SIZE;
	char len = this->name.size() + 1; // 1 por el /0
	memcpy(this->bytes + i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->bytes + i, (char*)this->name.c_str(), len);

	return this->bytes;
}

void District::setBytes(char* bytes)
{
	this->name = "";
	this->name.append(bytes + Constants::RECORD_HEADER_SIZE + Constants::FIELD_HEADER_SIZE);
}

District::~District()
{
	if(this->bytes != NULL)
	{
		delete this->bytes;
	}
}
