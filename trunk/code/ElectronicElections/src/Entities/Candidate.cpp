/*
 * Candidate.cpp
 *
 *  Created on: 04/10/2011
 *      Author: lejosdelcielo
 */


#include "Candidate.h"
#include "../VariableBlocks/Constants.h"
#include <cstring>

using namespace std;

Candidate::Candidate(char day, char month, short year, std::string name, std::string charge, int dni)
{
	this->day = day;
	this->month = month;
	this->year = year;
	this->charge = charge;
	this->name = name;
	this->dni = dni;
	this->bytes = NULL;
}

int Candidate::getSize()
{
	int size = 0;

	size += sizeof(this->year);
	size += sizeof(this->month);
	size += sizeof(this->day);
	size += Constants::FIELD_HEADER_SIZE + this->charge.size() + 1;
	size += Constants::FIELD_HEADER_SIZE + this->name.size() + 1;
	size += sizeof(this->dni);

	return size;
}

int Candidate::getKeySize()
{
	return this->getSize();
}

char* Candidate::getBytes()
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

	char chargeLen = this->charge.size() + 1;
	memcpy(this->bytes+i, &chargeLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->bytes+i, this->charge.c_str(), chargeLen); i += chargeLen;

	char nameLen = this->name.size() + 1;
	memcpy(this->bytes+i, &nameLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->bytes+i, this->name.c_str(), nameLen); i += nameLen;

	memcpy(this->bytes+i, &(this->dni), sizeof(int));

	return this->bytes;
}

char* Candidate::getKey()
{
	return this->getBytes();
}

void Candidate::setBytes(char* bytes)
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

	len = (bytes+i)[0]; i += sizeof(char);
	char nameAux[len];
	memcpy(nameAux, bytes+i, len); i+= len;
	this->name.clear();
	this->name.append(nameAux);

	memcpy(&(this->dni), bytes+i, sizeof(int));
}

// getters

short Candidate::getYear()
{
	return this->year;
}

char Candidate::getMonth()
{
	return this->month;
}

char Candidate::getDay()
{
	return this->day;
}

std::string Candidate::getCharge()
{
	return this->charge;
}

std::string Candidate::getName()
{
	return this->name;
}

int Candidate::getDni()
{
	return this->dni;
}

Candidate::~Candidate()
{
	if(this->bytes != NULL)
	{
		delete this->bytes;
	}
}
