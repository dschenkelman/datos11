/*
 * DistrictElectionMethods.cpp
 *
 *  Created on: Oct 16, 2011
 *      Author: damian
 */

#include "DistrictElectionMethods.h"
#include "../VariableBlocks/Constants.h"
#include <iostream>
#include <string.h>
#include "DistrictElections.h"

using namespace std;

DistrictElectionsMethods::DistrictElectionsMethods()
{
}

void DistrictElectionsMethods::printKey(const char *recordBytes, int recordSize)
{
	char districtSize = 0;
	memcpy(&districtSize, recordBytes, Constants::FIELD_HEADER_SIZE);
	char district[districtSize];
	memcpy(district, recordBytes + Constants::FIELD_HEADER_SIZE, districtSize);

	cout << district;
}

void DistrictElectionsMethods::print(const char *recordBytes, int recordSize)
{
	DistrictElections de;
	de.setBytes(recordBytes);

	cout << "(" << de.getDistrict() << ")";
}

int DistrictElectionsMethods::compare(const char *key, const char *recordBytes, int recordSize)
{
	DistrictElections de;
	de.setBytes(recordBytes);

	return strcmp(key, de.getDistrict().c_str());
}

VariableRecord *DistrictElectionsMethods::getKeyRecord(const char *recordBytes, int recordSize)
{
	DistrictElections de;
	de.setBytes(recordBytes);

	char districtLength = de.getDistrict().length() + 1;
	int rSize =  districtLength + Constants::FIELD_HEADER_SIZE;

	char buffer[rSize];
	memcpy(buffer, &districtLength, Constants::FIELD_HEADER_SIZE);
	memcpy(buffer + Constants::FIELD_HEADER_SIZE, de.getDistrict().c_str(), districtLength);

	VariableRecord* record = new VariableRecord();
	record->setBytes(buffer, rSize);

	return record;
}

DistrictElectionsMethods::~DistrictElectionsMethods()
{
}
