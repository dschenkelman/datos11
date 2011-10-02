/*
 * ElectionsList.cpp
 *
 *  Created on: 30/09/2011
 *      Author: lejosdelcielo
 */

#include "ElectionsList.h"
#include <stdlib.h>

ElectionsList::ElectionsList(std::string name, char day, char month, short year, std::string charge)
{
	this->name = name;
	this->day = day;
	this->month = month;
	this->year = year;
	this->charge = charge;
	this->bytes = NULL;
}

int ElectionsList::getSize()
{
	int nameBytes = this->name.size() + Constants::FIELD_HEADER_SIZE + 1;
	int chargeBytes = this->charge.size() + Constants::FIELD_HEADER_SIZE + 1;

	int ret = nameBytes + chargeBytes + sizeof(this->day) + sizeof(this->month)
			+ sizeof(this->year);

	return ret;
}

char* ElectionsList::getBytes()
{
	int size = this->getSize();

	if(this->bytes != NULL)
	{
		delete this->bytes;
	}

	this->bytes = new char[size];

	int i = 0;

	memcpy(this->bytes+i, &(this->year), sizeof(short)); i += sizeof(short);
	memcpy(this->bytes+i, &(this->month), sizeof(char)); i += sizeof(char);
	memcpy(this->bytes+i, &(this->day), sizeof(char)); i += sizeof(char);

	// campos cargo y longitud cargo
	char chargeLen = this->charge.size() + 1;
	memcpy(this->bytes+i, &chargeLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->bytes+i, this->charge.c_str(), chargeLen); i+= chargeLen;

	// campos nombre y longitud nombre
	char nameLen = this->name.size() + 1;
	memcpy(this->bytes+i, &nameLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->bytes+i, this->name.c_str(), nameLen);

	return bytes;
}

void ElectionsList::setBytes(char* bytes)
{
	int i = 0;

	short* yearAux = new short;
	memcpy(yearAux, bytes, sizeof(short)); i += sizeof(short);

	this->year = *yearAux;
	delete yearAux;
	this->month = (bytes+i)[0]; i += sizeof(char);
	this->day = (bytes+i)[0]; i += sizeof(char);

	char len = (bytes+i)[0]; i+= sizeof(char);
	char chargeAux[len];
	memcpy(chargeAux, bytes+i, len); i += len;
	this->charge.clear();
	this->charge.append(chargeAux);

	len = (bytes+i)[0]; i += sizeof(char);
	char nameAux[len];
	memcpy(nameAux, bytes+i, len); i+= len;
	this->name.clear();
	this->name.append(nameAux);
}

char ElectionsList::getDay()
{
	return this->day;
}

char ElectionsList::getMonth()
{
	return this->month;
}

short ElectionsList::getYear()
{
	return this->year;
}

std::string ElectionsList::getName()
{
	return this->name;
}

std::string ElectionsList::getCharge()
{
	return this->charge;
}

ElectionsList::~ElectionsList()
{
	if(this->bytes != NULL)
	{
		delete this->bytes;
	}
}

