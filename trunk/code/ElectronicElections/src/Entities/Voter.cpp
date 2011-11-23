/*
 * Voter.cpp
 *
 *  Created on: 07/10/2011
 *      Author: lejosdelcielo
 */

static int PASSWORD_SIZE = 4; //sizeof int64 for crypted passwords

#include "Voter.h"
#include "../VariableBlocks/Constants.h"
#include <cstring>

Voter::Voter(int dni, std::string names, std::string password, std::string address,
		std::string district, std::vector<ElectionKey> electionKeyList)
{
	this->dni = dni;
	this->names = names;
	this->password = password;
	this->address = address;
	this->district = district;
	this->electionKeyList = electionKeyList;
	this->bytes = NULL;
	this->key = NULL;
}


Voter::Voter(int dni, std::string names, std::string password, std::string address, std::string district)
{
	this->dni = dni;
	this->names = names;
	this->password = password;
	this->address = address;
	this->district = district;
	this->electionKeyList = electionKeyList;
	this->bytes = NULL;
	this->key = NULL;
}

int Voter::getSize()
{
	int size = 0;
	size += sizeof(this->dni);
	size += Constants::FIELD_HEADER_SIZE + 1 + this->names.size();
	size += Constants::FIELD_HEADER_SIZE + 1 + this->password.size();

	size += Constants::FIELD_HEADER_SIZE + 1 + this->address.size();
	size += Constants::FIELD_HEADER_SIZE + 1+ this->district.size();
	size += Constants::FIELD_HEADER_SIZE;

	for(int i = 0; i < this->electionKeyList.size(); i++)
	{
		size += Constants::FIELD_HEADER_SIZE + 1 + this->electionKeyList.at(i).charge.size();
		size += sizeof(this->electionKeyList.at(i).year);
		size += sizeof(this->electionKeyList.at(i).month);
		size += sizeof(this->electionKeyList.at(i).day);
	}

	return size;
}

int Voter::getKeySize()
{
	return sizeof(this->dni);
}

char* Voter::getKey()
{
	int size = this->getKeySize();

	if(this->key != NULL)
	{
		delete[] this->key;
	}

	this->key = new char[size];

	memcpy(this->key, &(this->dni), size);

	return this->key;
}

void Voter::setBytes(char* bytes)
{
	int i = 0;

	memcpy(&this->dni, bytes+i, sizeof(int)); i += sizeof(int);
	char len = (bytes+i)[0]; i += Constants::FIELD_HEADER_SIZE;
	char nameAux[len];
	memcpy(nameAux, bytes+i, len); i += len;
	this->names.clear(); this->names.append(nameAux);

	int passwdSize = (int) *(bytes+i);
	char pass[passwdSize]; i += Constants::FIELD_HEADER_SIZE;
//	memset(pass, 0, PASSWORD_SIZE+1);
	memcpy(pass, bytes+i, passwdSize); i += passwdSize;

	this->password.clear();
	this->password.append(pass);

//	this->password.clear();
//	std::string passStrAux = "";
//	if (pass != '\0')
//	{
//		passStrAux = pass;
//	}
//
//	this->password.clear();
//	this->password.append(passStrAux.substr(0, 4));

	len = (bytes+i)[0]; i += Constants::FIELD_HEADER_SIZE;
	char address[len];
	memcpy(address, bytes+i, len); i += len;
	this->address.clear(); this->address.append(address);

	len = (bytes+i)[0]; i += Constants::FIELD_HEADER_SIZE;
	char district[len];
	memcpy(district, bytes+i, len); i += len;
	this->district.clear(); this->district.append(district);

	char listSize = (bytes+i)[0]; i += Constants::FIELD_HEADER_SIZE;
	char elements = 0;

	this->electionKeyList.clear();
	while(elements < listSize)
	{
		short year; char month, day;
		memcpy(&year, bytes+i, sizeof(short)); i += sizeof(short);
		memcpy(&month, bytes+i, sizeof(char)); i += sizeof(char);
		memcpy(&day, bytes+i, sizeof(char)); i += sizeof(char);

		char elementLen = (bytes+i)[0]; i += Constants::FIELD_HEADER_SIZE;
		char chargeAux[len];
		memcpy(chargeAux, bytes+i, elementLen); i += elementLen;
		std::string charge = chargeAux;

		ElectionKey electionKey = {year, month, day, charge};
		this->electionKeyList.push_back(electionKey);
		elements++;
	}
}

//(DNI, lenNA, NombreyApellido, clave, lenD, domicilio, lenDi, district,
//lenlist, listElectionKey)
char* Voter::getBytes()
{
	int size = this->getSize();

	if(this->bytes != NULL)
	{
		delete[] this->bytes;
	}

	this->bytes = new char[size];
	int i = 0;

	memcpy(this->bytes+i, &(this->dni), sizeof(int)); i += sizeof(int);

	char len = this->names.size() + 1;
	memcpy(this->bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->bytes+i, this->names.c_str(), len); i += len;

	len = this->password.size() + 1;
	memcpy(this->bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->bytes+i, this->password.c_str(), len); i += len;

//	memcpy(this->bytes+i, this->password.c_str(), PASSWORD_SIZE+1); i += PASSWORD_SIZE+1;


	len = this->address.size() + 1;
	memcpy(this->bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->bytes+i, this->address.c_str(), len); i += len;

	len = this->district.size() + 1;
	memcpy(this->bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(this->bytes+i, this->district.c_str(), len); i += len;

	char listSize = this->electionKeyList.size();
	memcpy(this->bytes+i, &listSize, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;

	for(int j = 0; j < this->electionKeyList.size(); j++)
	{
		memcpy(this->bytes+i, &(this->electionKeyList.at(j).year), sizeof(short)); i += sizeof(short);
		memcpy(this->bytes+i, &(this->electionKeyList.at(j).month), sizeof(char)); i += sizeof(char);
		memcpy(this->bytes+i, &(this->electionKeyList.at(j).day), sizeof(char)); i += sizeof(char);

		len = this->electionKeyList.at(j).charge.size() + 1;
		memcpy(this->bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
		memcpy(this->bytes+i, this->electionKeyList.at(j).charge.c_str(), len); i += len;
	}

	return this->bytes;
}

int Voter::getDni()
{
	return this->dni;
}

std::string Voter::getNames()
{
	return this->names;
}

void Voter::setNames(std::string names) {
	this->names = names;
}

std::string Voter::getPassword()
{
	return this->password;
}

void Voter::setPassword(std::string password) {
	this->password = password;
}

std::string Voter::getAddress()
{
	return this->address;
}

void Voter::setAddress(std::string address) {
	this->address = address;
}

std::string Voter::getDistrict()
{
	return this->district;
}

void Voter::setDistrict(std::string district) {
	this->district = district;
}

Voter::Voter(const Voter & other)
{
	this->key = NULL;
	this->bytes = NULL;
	this->address = other.address;
	this->district = other.district;
	this->dni = other.dni;
	this->electionKeyList = other.electionKeyList;
	this->names = other.names;
	this->password = other.password;
}

Voter & Voter::operator =(const Voter & other)
{
	if (this == &other)
	{
		return *this;
	}

	this->address = other.address;
	this->district = other.district;
	this->dni = other.dni;
	this->electionKeyList = other.electionKeyList;
	this->names = other.names;
	this->password = other.password;

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

std::vector<ElectionKey>& Voter::getElectionKeyList()
{
	return this->electionKeyList;
}

#include <iostream>
#include "../Helpers/base64.h"
#include <math.h>

void Voter::setEncBytes(char* encBytes, RSAKey privateKey, int originalSize) {
	RSACipher rsac;
	char* tmpBytes = new char[1024];
	int chunkSize = rsac.getChunkSize(privateKey.n) + 1;
	int chunks = ceil(originalSize / (float)(chunkSize - 1));
	rsac.decryptMessage(encBytes, privateKey.exp, privateKey.n, tmpBytes, chunks * chunkSize);
	/* BINARY DEBUG PRINT */
	cout << "DECRYPTED: " << base64_encode((const unsigned char*)tmpBytes, 20) << endl;
	cout.flush();
	/* END BINARY DEBUG PRINT */
	this->setBytes(tmpBytes);
}
void Voter::getEncBytes(char* encBytes, RSAKey publicKey) {
	RSACipher rsac;
	rsac.cipherMessage(this->getBytes(), publicKey.exp, publicKey.n, encBytes, this->getSize());
}

Voter::~Voter()
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

