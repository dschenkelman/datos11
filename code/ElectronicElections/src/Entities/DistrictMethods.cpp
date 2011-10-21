/*
 * DistrictMethods.cpp
 *
 *  Created on: 28/09/2011
 *      Author: alejandro
 */

#include "DistrictMethods.h"
#include <string.h>
#include <iostream>
#include "../VariableBlocks/Constants.h"

using namespace std;

//DEFINITION:
//char[FIELD_HEADER_SIZE] name_size
//char[name_size] name // ending with \0

DistrictMethods::DistrictMethods() {

}

DistrictMethods::~DistrictMethods() {
}

int DistrictMethods::compareKey(const char* key, const char* recordKey, int recordSize)
{
	return this->compare(key, recordKey, recordSize);
}

int DistrictMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
	District district("district");
	district.setBytes((char*)recordBytes);

	int size = strlen(key + Constants::FIELD_HEADER_SIZE) + 1;
	char name[size];
	memcpy(name, key + Constants::FIELD_HEADER_SIZE, size);

	return strcmp(name, district.getName().c_str());
}

void DistrictMethods::print(const char* recordBytes, int recordSize)
{
	District district("district");
	district.setBytes((char*) recordBytes);

	cout <<"(" <<  district.getName() << ")";
}

void DistrictMethods::printKey(const char* key, int recordSize)
{
	char len;
	memcpy(&len, key, Constants::FIELD_HEADER_SIZE);
	char buffer[len];
	memcpy(&buffer, key + Constants::FIELD_HEADER_SIZE, len);
	cout << buffer;
}

VariableRecord *DistrictMethods::getKeyRecord(const char *recordBytes, int recordSize)
{
	VariableRecord* record = new VariableRecord();
	record->setBytes(recordBytes, recordSize);

	return record;
}
