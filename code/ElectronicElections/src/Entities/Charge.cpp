/*
 * Charge.cpp
 *
 *  Created on: 06/10/2011
 *      Author: lejosdelcielo
 */

#include "Charge.h"
#include "../VariableBlocks/Constants.h"
#include <cstring>

Charge::Charge(std::string charge, std::vector<std::string> chargeList)
{
	this->charge = charge;
	this->chargeList = chargeList;
	this->bytes = NULL;
	this->key = NULL;
}

int Charge::getSize()
{
	int size = 0;
	size += Constants::FIELD_HEADER_SIZE + this->charge.size() + 1;

	size += Constants::FIELD_HEADER_SIZE;

	for(int i = 0; i < this->chargeList.size(); i++)
	{
		size += Constants::FIELD_HEADER_SIZE + 1 + this->chargeList.at(i).size();
	}

	return size;
}

int Charge::getKeySize()
{
	int size = Constants::FIELD_HEADER_SIZE + this->charge.size() + 1;
	return size;
}

char* Charge::getKey()
{
	int keySize = this->getKeySize();

	if(this->key != NULL)
	{
		delete[] this->key;
	}

	this->key = new char[keySize];

	int i = 0;

	char len = this->charge.size() + 1;
	memcpy(this->key+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->key+i, this->charge.c_str(), len);

	return this->key;
}

void Charge::setBytes(char* bytes)
{
	int i = 0;

	char len = (bytes+i)[0]; i += Constants::FIELD_HEADER_SIZE;
	char chargeAux[len];
	memcpy(chargeAux, bytes+i, len); i += len;
	this->charge.clear(); this->charge.append(chargeAux);

	char listSize = (bytes+i)[0]; i += Constants::FIELD_HEADER_SIZE;
	char elements = 0;

	this->chargeList.clear();
	while(elements < listSize)
	{
		len = (bytes+i)[0]; i += Constants::FIELD_HEADER_SIZE;
		char aux[len];
		memcpy(aux, bytes+i, len); i += len;
		std::string c = aux;
		this->chargeList.push_back(c);
		elements++;
	}
}

char* Charge::getBytes()
{
	int size = this->getSize();

	if(this->bytes != NULL)
	{
		delete[] this->bytes;
	}

	this->bytes = new char[size];

	char len = this->charge.size() + 1;
	int i = 0;

	memcpy(this->bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->bytes+i, this->charge.c_str(), len); i += len;

	char listSize = this->chargeList.size();
	memcpy(this->bytes+i, &listSize, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;

	for(int j = 0; j < this->chargeList.size(); j++)
	{
		len = 1 + this->chargeList.at(j).size();
		memcpy(this->bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
		memcpy(this->bytes+i, this->chargeList.at(j).c_str(), len); i += len;
	}

	return this->bytes;
}

std::string Charge::getCharge()
{
	return this->charge;
}

Charge & Charge::operator =(const Charge & other)
{
	if (this == &other)
	{
		return *this;
	}

	this->chargeList = other.chargeList;
	this->charge = other.charge;

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

Charge::Charge(const Charge & other)
{
	this->bytes = NULL;
	this->key = NULL;
	this->chargeList = other.chargeList;
	this->charge = other.charge;
}

std::vector<std::string> Charge::getChargeList()
{
	return this->chargeList;
}

Charge::~Charge()
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

