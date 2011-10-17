/*
 * DistrictCountsMethods.cpp
 *
 *  Created on: Oct 17, 2011
 *      Author: damian
 */

#include "DistrictCountsMethods.h"
#include "DistrictCounts.h"
#include <iostream>
#include <string.h>
#include "../VariableBlocks/Constants.h"

using namespace std;

DistrictCountsMethods::DistrictCountsMethods()
{
}

int DistrictCountsMethods::compare(const char *key, const char *recordBytes, int recordSize)
{
	DistrictCounts dc;
	dc.setBytes(recordBytes);

	return strcmp(key, dc.getDistrict().c_str());
}

VariableRecord *DistrictCountsMethods::getKeyRecord(const char *recordBytes, int recordSize)
{
	DistrictCounts dc;
	dc.setBytes(recordBytes);

	char districtLength = dc.getDistrict().length() + 1;
	int rSize =  districtLength + Constants::FIELD_HEADER_SIZE;

	char buffer[rSize];
	memcpy(buffer, &districtLength, Constants::FIELD_HEADER_SIZE);
	memcpy(buffer + Constants::FIELD_HEADER_SIZE, dc.getDistrict().c_str(), districtLength);

	VariableRecord* record = new VariableRecord();
	record->setBytes(buffer, rSize);

	return record;
}

void DistrictCountsMethods::printKey(const char *recordBytes, int recordSize)
{
	char districtSize = 0;
	memcpy(&districtSize, recordBytes, Constants::FIELD_HEADER_SIZE);
	char district[districtSize];
	memcpy(district, recordBytes + Constants::FIELD_HEADER_SIZE, districtSize);

	cout << district;
}

void DistrictCountsMethods::print(const char *recordBytes, int recordSize)
{
	DistrictCounts dc;
	dc.setBytes(recordBytes);

	cout << "(" << dc.getDistrict() << ")";
}

DistrictCountsMethods::~DistrictCountsMethods()
{
}
