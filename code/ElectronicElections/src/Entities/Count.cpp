/*
 * Count.cpp
 *
 *  Created on: 05/10/2011
 *      Author: lejosdelcielo
 */

#include "Count.h"
#include "../VariableBlocks/Constants.h"
#include <string>
#include <cstring>

Count::Count(char day, char month, short year, std::string charge, std::string name, std::string district, unsigned int quantity)
{
	this->day = day;
	this->month = month;
	this->year = year;
	this->charge = charge;
	this->name = name;
	this->district = district;
	this->quantity = quantity;
	this->bytes = NULL;
	this->key = NULL;
}

int Count::getSize()
{
	int size = 0;
	size += sizeof(this->year) + sizeof(this->month) + sizeof(this->day);
	size += Constants::FIELD_HEADER_SIZE + this->charge.size() + 1;
	size += Constants::FIELD_HEADER_SIZE + this->name.size() + 1;
	size += Constants::FIELD_HEADER_SIZE + this->district.size() + 1;
	size += sizeof(this->quantity);

	return size;
}

int Count::getKeySize()
{
	int size = 0;
	size += sizeof(this->year) + sizeof(this->month) + sizeof(this->day);
	size += Constants::FIELD_HEADER_SIZE + this->charge.size() + 1;
	size += Constants::FIELD_HEADER_SIZE + this->name.size() + 1;
	size += Constants::FIELD_HEADER_SIZE + this->district.size() + 1;

	return size;
}

char* Count::getBytes()
{
	int size = this->getSize();

	if(this->bytes != NULL)
	{
		delete[] this->bytes;
	}

	this->bytes = new char[size];

	char* keyAux = this->getKey();
	int i = this->getKeySize();
	memcpy(this->bytes, keyAux, i);

	memcpy(this->bytes+i, &(this->quantity), sizeof(unsigned int));

	return this->bytes;
}

char* Count::getKey()
{
	int size = this->getKeySize();

	if(this->key != NULL)
	{
		delete[] this->key;
	}

	this->key = new char[size];
	memset(this->key, 0, size);

	int i = 0;

	memcpy(this->key+i, &(this->year), sizeof(short)); i += sizeof(short);
	memcpy(this->key+i, &(this->month), sizeof(char)); i += sizeof(char);
	memcpy(this->key+i, &(this->day), sizeof(char)); i += sizeof(char);

	char chargeLen = this->charge.size() + 1;
	memcpy(this->key+i, &chargeLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->key+i, this->charge.c_str(), chargeLen); i += chargeLen;

	char nameLen = this->name.size() + 1;
	memcpy(this->key+i, &nameLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->key+i, this->name.c_str(), nameLen); i += nameLen;

	char districtLen = this->district.size() + 1;
	memcpy(this->key+i, &districtLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->key+i, this->district.c_str(), districtLen);

	return this->key;
}

void Count::setBytes(char* bytes)
{
	int i = 0;

	memcpy(&(this->year), bytes+i, sizeof(short)); i += sizeof(short);
	this->month = (bytes+i)[0]; i += sizeof(char);
	this->day = (bytes+i)[0]; i += sizeof(char);

	char chargeLen = (bytes+i)[0]; i += sizeof(char);
	char chargeAux[chargeLen];
	memcpy(chargeAux, bytes+i, chargeLen); i += chargeLen;
	this->charge.clear(); this->charge.append(chargeAux);

	char nameLen = (bytes+i)[0]; i += sizeof(char);
	char nameAux[nameLen];
	memcpy(nameAux, bytes+i, nameLen); i += nameLen;
	this->name.clear(); this->name.append(nameAux);

	char districtLen = (bytes+i)[0]; i += sizeof(char);
	char districtAux[districtLen];
	memcpy(districtAux, bytes+i, districtLen); i += districtLen;
	this->district.clear(); this->district.append(districtAux);

	memcpy(&(this->quantity), bytes+i, sizeof(unsigned int));
}

short Count::getYear()
{
	return this->year;
}

char Count::getMonth()
{
	return this->month;
}

char Count::getDay()
{
	return this->day;
}

std::string Count::getCharge()
{
	return this->charge;
}

std::string Count::getListName()
{
	return this->name;
}

std::string Count::getDistrict()
{
	return this->district;
}

unsigned int Count::getQuantity()
{
	return this->quantity;
}

Count::~Count()
{
	if(this->key != NULL)
	{
		delete[] this->key;
	}

	if(this->bytes != NULL)
	{
		delete[] this->bytes;
	}
}

