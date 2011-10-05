/*
 * Administrator.cpp
 *
 *  Created on: 04/10/2011
 *      Author: lejosdelcielo
 */

#include "Administrator.h"
#include "../VariableBlocks/Constants.h"
#include <cstring>

Administrator::Administrator(std::string name, std::string password)
{
	this->name = name;
	this->password.append(password, 0, PASSWORD_SIZE);
	this->bytes = NULL;
	this->key = NULL;
}

int Administrator::getSize()
{
	int size = 0;

	size += this->name.size() + 1 + Constants::FIELD_HEADER_SIZE;
	size += this->password.size() + 1;

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
		delete this->bytes;
	}

	this->bytes = new char[size];

	int i = 0;

	char len = this->name.size() + 1;
	memcpy(this->bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->bytes+i, this->name.c_str(), len); i+= len;
	len = this->password.size() + 1;
	memcpy(this->bytes+i, this->password.c_str(), len);

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

	char passAux[PASSWORD_SIZE];
	memcpy(passAux, bytes+i, PASSWORD_SIZE);

	std::string passStrAux = "";
	passStrAux.append(passAux);
	this->password.clear();
	this->password.append(passStrAux.substr(0, 4));
}

char* Administrator::getKey()
{
	if(this->key != NULL)
	{
		delete this->key;
	}

	int size = getKeySize();
	this->key = new char[size];

	memcpy(key, &size, Constants::FIELD_HEADER_SIZE);
	memcpy(key+Constants::FIELD_HEADER_SIZE, this->name.c_str(), size);

	return this->key;
}

std::string Administrator::getPassword()
{
	return this->password;
}

std::string Administrator::getName()
{
	return this->name;
}

Administrator::~Administrator()
{
	if(this->bytes != NULL)
	{
		delete this->bytes;
	}

	if(this->key != NULL)
	{
		delete this->key;
	}
}

