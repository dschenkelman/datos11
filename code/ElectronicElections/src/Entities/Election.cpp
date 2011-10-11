/*
 * Election.cpp
 *
 *  Created on: 06/10/2011
 *      Author: lejosdelcielo
 */

#include "Election.h"
#include "../VariableBlocks/Constants.h"
#include <cstring>

Election::Election(char day, char month, short year, std::string charge, std::vector<string> districtList)
{
	this->day = day;
	this->month = month;
	this->year = year;
	this->charge = charge;
	this->districtList = districtList;
	this->bytes = NULL;
	this->key = NULL;
}

int Election::getSize()
{
	int size = 0;

	size += sizeof(this->day) + sizeof(this->month) + sizeof(this->year);
	size += Constants::FIELD_HEADER_SIZE + 1 + this->charge.size();
	size += Constants::FIELD_HEADER_SIZE;

	for(int i = 0; i < this->districtList.size(); i++)
	{
		size += Constants::FIELD_HEADER_SIZE;
		size += this->districtList.at(i).size() + 1;
	}

	return size;
}

int Election::getKeySize()
{
	int size = 0;

	size += sizeof(this->day) + sizeof(this->month) + sizeof(this->year);
	size += Constants::FIELD_HEADER_SIZE + 1 + this->charge.size();

	return size;
}

void Election::setBytes(char* bytes)
{
	int i = 0;

	memcpy(&(this->year), bytes+i, sizeof(short)); i += sizeof(short);
	this->month = (bytes+i)[0]; i += sizeof(char);
	this->day = (bytes+i)[0]; i += sizeof(char);

	char len = (bytes+i)[0]; i+= sizeof(char);
	char chargeAux[len];
	memcpy(chargeAux, bytes+i, len); i += len;
	this->charge.clear();
	this->charge.append(chargeAux);

	char listSize = (bytes+i)[0]; i += Constants::FIELD_HEADER_SIZE;
	char elements = 0;

	this->districtList.clear();
	while(elements < listSize)
	{
		len = (bytes+i)[0]; i += Constants::FIELD_HEADER_SIZE;
		char aux[len];
		memcpy(aux, bytes+i, len); i += len;
		std::string d = aux;
		this->districtList.push_back(d);
		elements++;
	}
}

char* Election::getBytes()
{
	int size = this->getSize();

	if(this->bytes != NULL)
	{
		delete[] this->bytes;
	}

	this->bytes = new char[size];

	int i = 0;

	memcpy(this->bytes+i, &(this->year), sizeof(short)); i += sizeof(short);
	memcpy(this->bytes+i, &(this->month), sizeof(char)); i += sizeof(char);
	memcpy(this->bytes+i, &(this->day), sizeof(char)); i += sizeof(char);

	char chargeLen = this->charge.size() + 1;
	memcpy(this->bytes+i, &chargeLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->bytes+i, this->charge.c_str(), chargeLen); i += chargeLen;

	char listSize = this->districtList.size();
	memcpy(this->bytes+i, &listSize, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;

	for(int j = 0; j < this->districtList.size(); j++)
	{
		char len = this->districtList.at(j).size() + 1;
		memcpy(this->bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
		memcpy(this->bytes+i, this->districtList.at(j).c_str(), len); i += len;
	}

	return this->bytes;
}

char* Election::getKey()
{
	int size = this->getKeySize();

	if(this->key != NULL)
	{
		delete[] this->key;
	}

	this->key = new char[size];

	int i = 0;

	memcpy(this->key+i, &(this->year), sizeof(short)); i += sizeof(short);
	memcpy(this->key+i, &(this->month), sizeof(char)); i += sizeof(char);
	memcpy(this->key+i, &(this->day), sizeof(char)); i += sizeof(char);

	char chargeLen = this->charge.size() + 1;
	memcpy(this->key+i, &chargeLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->key+i, this->charge.c_str(), chargeLen); i += chargeLen;

	return this->key;
}

char Election::getDay()
{
	return this->day;
}

char Election::getMonth()
{
	return this->month;
}

short Election::getYear()
{
	return this->year;
}

std::vector<std::string> Election::getDistrictList()
{
	return this->districtList;
}

std::string Election::getCharge()
{
	return this->charge;
}

Election::~Election()
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

