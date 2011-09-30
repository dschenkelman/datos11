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
	return this->name.size() + Constants::FIELD_HEADER_SIZE + 1;
}

char* District::getKey()
{
	return this->getBytes();
}

char* District::getBytes()
{
	int size = this->getSize();

	if(this->bytes != NULL)
	{
		delete this->bytes;
	}

	this->bytes = new char[size];

	size -= Constants::FIELD_HEADER_SIZE;

	memcpy(this->bytes, &size, sizeof(char));
	memcpy(this->bytes+1, (char*)this->name.c_str(), size);

	return this->bytes;
}

void District::setBytes(char* bytes)
{
	this->name = "";
	this->name.append(bytes+1);
}

District::~District()
{
	if(this->bytes != NULL)
	{
		delete this->bytes;
	}
}
