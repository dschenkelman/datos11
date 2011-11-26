/*
 * AdministratorMethods.cpp
 *
 *  Created on: 04/10/2011
 *      Author: lejosdelcielo
 */

#include "AdministratorMethods.h"
#include "Administrator.h"
#include "../VariableBlocks/Constants.h"
#include <iostream>
#include <cstring>

using namespace std;

AdministratorMethods::AdministratorMethods()
{
}

int AdministratorMethods::compareKey(const char* key, const char* recordKey, int recordSize)
{
	return this->compare(key, recordKey, recordSize);
}

int AdministratorMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
	vector<char> passVector;
	Administrator adm("Adm", passVector);
	adm.setBytes((char*)recordBytes);

	int size = strlen(key + Constants::FIELD_HEADER_SIZE) + 1;
	char name[size];

	memcpy(name, key + Constants::FIELD_HEADER_SIZE, size);

	return strcmp(name, adm.getName().c_str());
}

void AdministratorMethods::print(const char* recordBytes, int recordSize)
{
	vector<char> passVector;
	Administrator adm("adm", passVector);
	adm.setBytes((char*)recordBytes);

//	cout << "(" << adm.getName() << "," << adm.getPassword() << ")";
	vector<char> passwordVector = adm.getPassword();
	char aux[passwordVector.size() + 1];
	for (int i = 0; i < passwordVector.size(); i++)
	{
		aux[i] = passwordVector[i];
	}

	aux[passwordVector.size()] = '\0';

	cout << "Nombre: " << adm.getName() << " Contraseña: " << aux << endl;
}

void AdministratorMethods::printKey(const char* key, int recordSize)
{
	char len;
	memcpy(&len, key, Constants::FIELD_HEADER_SIZE);
	char buffer[len];
	memcpy(buffer, key + Constants::FIELD_HEADER_SIZE, len);

	cout << buffer;
}

VariableRecord* AdministratorMethods::getKeyRecord(const char* recordBytes, int recordSize)
{
	char len;
	memcpy(&len, recordBytes, Constants::FIELD_HEADER_SIZE);
	char buffer[len + Constants::FIELD_HEADER_SIZE];
	memcpy(buffer, &len, Constants::FIELD_HEADER_SIZE);
	memcpy(buffer + Constants::FIELD_HEADER_SIZE, recordBytes + Constants::FIELD_HEADER_SIZE, len);
	VariableRecord* record = new VariableRecord();
	record->setBytes(buffer, len);

	return record;
}

AdministratorMethods::~AdministratorMethods()
{
}

