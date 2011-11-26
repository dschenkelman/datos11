/*
 * Administrator.cpp
 *
 *  Created on: 04/10/2011
 *      Author: lejosdelcielo
 */

#include "Administrator.h"
#include "../VariableBlocks/Constants.h"
#include <cstring>

Administrator::Administrator(std::string name, std::vector<char> password)
{
	this->name = name;
	this->password = password;
	this->bytes = NULL;
	this->key = NULL;
}

int Administrator::getSize()
{
	int size = 0;

	size += this->name.size() + 1 + Constants::FIELD_HEADER_SIZE;
	size += this->password.size()+ Constants::FIELD_HEADER_SIZE;

	return size;
}

int Administrator::getKeySize()
{
	return this->name.size() + 1 + Constants::FIELD_HEADER_SIZE;
}

char* Administrator::getBytes()
{
	int size = this->getSize();

	if(this->bytes != NULL)
	{
		delete[] this->bytes;
	}

	this->bytes = new char[size];

	int i = 0;

	char len = this->name.size() + 1;
	memcpy(this->bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->bytes+i, this->name.c_str(), len); i+= len;
	len = this->password.size();
	memcpy(this->bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;

	char buffer[len];
	for (int i = 0; i < len; i++)
	{
		buffer[i] = this->password[i];
	}
	memcpy(this->bytes+i, buffer, len); i+= len;

	return this->bytes;
}

void Administrator::setBytes(char* bytes)
{
	char len = 0;
	int i = 0;

	len = (bytes+i)[0]; i += Constants::FIELD_HEADER_SIZE;
	char nameAux[len];
	memcpy(nameAux, bytes+i, len); i+= len;
	this->name.clear();
	this->name.append(nameAux);

	len = (bytes+i)[0]; i += Constants::FIELD_HEADER_SIZE;
	char passAux[len];
	memcpy(passAux, bytes+i, len); i+= len;
	this->password.clear();
	for (int i = 0; i < len; i++)
	{
		this->password.push_back(passAux[i]);
	}
}

char* Administrator::getKey()
{
	if(this->key != NULL)
	{
		delete []this->key;
	}

	int size = getKeySize();
	this->key = new char[size];

	memcpy(this->key, &size, Constants::FIELD_HEADER_SIZE);
	memcpy(this->key+Constants::FIELD_HEADER_SIZE, this->name.c_str(), size - Constants::FIELD_HEADER_SIZE);

	return this->key;
}

std::vector<char> Administrator::getPassword()
{
	return this->password;
}

std::string Administrator::getName()
{
	return this->name;
}

Administrator& Administrator::operator =(const Administrator & other)
{
	if (this == &other)
	{
		return *this;
	}

	this->password = other.password;
	this->name = other.name;

	if (this->key != NULL)
	{
		delete[] this->key;
		this->key = NULL;
	}

	if (this->bytes != NULL)
	{
		delete[] this->bytes;
		this->bytes = NULL;
	}

	return *this;
}

Administrator::Administrator(const Administrator & other)
{
	this->bytes = NULL;
	this->key = NULL;
	this->password = other.password;
	this->name = other.name;

}

void Administrator::setPassword(std::vector<char>& value)
{
	this->password = value;
}

Administrator::~Administrator()
{
	if(this->bytes != NULL)
	{
		delete[] this->bytes;
	}

	if(this->key != NULL)
	{
		delete[] this->key;
	}
}
