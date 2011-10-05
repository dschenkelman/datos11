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

using namespace std;

AdministratorMethods::AdministratorMethods()
{
}

int AdministratorMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
	Administrator adm("Adm", "adm");
	adm.setBytes((char*)recordBytes);

	int size = strlen(key + Constants::FIELD_HEADER_SIZE) + 1;
	char name[size];

	memcpy(name, key + Constants::FIELD_HEADER_SIZE, size);

	return strcmp(name, adm.getName().c_str());
}

void AdministratorMethods::print(const char* recordBytes, int recordSize)
{
	Administrator adm("adm", "adm");
	adm.setBytes((char*)recordBytes);

	cout << adm.getName() << adm.getPassword() << endl;
}

void AdministratorMethods::printKey(const char* key, int recordSize)
{
	char len;
	memcpy(&len, key, Constants::FIELD_HEADER_SIZE);
	char buffer[len];
	memcpy(buffer, key + Constants::FIELD_HEADER_SIZE, len);

	cout << buffer << endl;
}

VariableRecord* AdministratorMethods::getKeyRecord(const char* recordBytes, int recordSize)
{
	VariableRecord record = new VariableRecord();
	record.setBytes(recordBytes, recordSize);

	return record;
}

AdministratorMethods::~AdministratorMethods()
{
}

