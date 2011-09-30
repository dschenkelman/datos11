/*
 * DistrictMethods.cpp
 *
 *  Created on: 28/09/2011
 *      Author: alejandro
 */

#include "DistrictMethods.h"

//DEFINITION:
//char[FIELD_HEADER_SIZE] name_size
//char[name_size] name // ending with \0


DistrictMethods::DistrictMethods() {

}

DistrictMethods::~DistrictMethods() {
}

int DistrictMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
	District district("district");
	district.setBytes((char*)recordBytes);

	return strcmp(key, district.getKey());
}

void DistrictMethods::print(const char* recordBytes, int recordSize)
{
	District district("district");
	district.setBytes((char*) recordBytes);

	cout << district.getName() << endl;
}
