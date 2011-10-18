/*
 * CandidatesList.cpp
 *
 *  Created on: 08/10/2011
 *      Author: alejandro
 */

#include "CandidatesList.h"
#include "../VariableBlocks/Constants.h"
#include <string>
#include <cstring>

CandidatesList::CandidatesList(char day, char month, short year, std::string charge, std::string name)
{
	this->day = day;
	this->month = month;
	this->year = year;
	this->charge = charge;
	this->name = name;
	this->bytes = NULL;
	this->key = NULL;
}

int CandidatesList::getSize()
{
	int size = 0;
	size += sizeof(this->year) + sizeof(this->month) + sizeof(this->day);
	size += Constants::FIELD_HEADER_SIZE + this->charge.size() + 1;
	size += Constants::FIELD_HEADER_SIZE + this->name.size() + 1;

	return size;
}

int CandidatesList::getKeySize()
{
	// es _todo indice
	return getSize();
}

char* CandidatesList::getBytes()
{
	int size = this->getSize();

	if(this->bytes != NULL)
	{
		delete this->bytes;
	}

	this->bytes = new char[size];

	char* keyAux = this->getKey();
	int i = this->getKeySize();
	memcpy(this->bytes, keyAux, i);

	return this->bytes;
}

char* CandidatesList::getKey()
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

	return this->key;
}

void CandidatesList::setBytes(char* bytes)
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
}

short CandidatesList::getYear()
{
	return this->year;
}

char CandidatesList::getMonth()
{
	return this->month;
}

char CandidatesList::getDay()
{
	return this->day;
}

std::string CandidatesList::getCharge()
{
	return this->charge;
}

CandidatesList & CandidatesList::operator =(const CandidatesList & other)
{
	if (this == &other)
	{
		return *this;
	}

	this->charge = other.charge;
	this->day = other.day;
	this->month = other.month;
	this->year = other.year;
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

CandidatesList::CandidatesList(const CandidatesList & other)
{
	this->bytes = NULL;
	this->key = NULL;
	this->charge = other.charge;
	this->day = other.day;
	this->month = other.month;
	this->year = other.year;
	this->name = other.name;
}

std::string CandidatesList::getListName()
{
	return this->name;
}

CandidatesList::~CandidatesList()
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

