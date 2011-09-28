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
	char bytes[size];

	size -= Constants::FIELD_HEADER_SIZE;

	memcpy(bytes, &size, sizeof(char));
	memcpy(bytes+1, this->name.c_str(), size);

	return bytes;
}

void District::setBytes(char* bytes)
{
	this->name = "";
	this->name.append(bytes+1);
}

District::~District()
{
}
