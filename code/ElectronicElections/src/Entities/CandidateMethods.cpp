/*
 * CandidateMethods.cpp
 *
 *  Created on: 04/10/2011
 *      Author: lejosdelcielo
 */

#include "CandidateMethods.h"
#include "Candidate.h"
#include "../VariableBlocks/Constants.h"
#include <iostream>
#include <cstring>

using namespace std;

CandidateMethods::CandidateMethods()
{
}

int CandidateMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
	Candidate c(1, 1, 1, "invalid", "invalid", 1);
	c.setBytes((char*) recordBytes);

	short year = 0, i = 0;
	memcpy(&year, key, sizeof(short)); i+= sizeof(short);
	char month = 0;
	memcpy(&month, key+i, sizeof(char)); i+= sizeof(char);
	char day = 0;
	memcpy(&day, key+i, sizeof(char)); i+= sizeof(char);

	char chargeLen = 0;
	memcpy(&chargeLen, key+i, Constants::FIELD_HEADER_SIZE); i+= Constants::FIELD_HEADER_SIZE;
	char charge[chargeLen];
	memcpy(&charge, key+i, chargeLen); i += chargeLen;

	if(year > c.getYear())
	{
		return 1;
	}

	else
	{
		if(year < c.getYear())
		{
			return -1;
		}

		if(month > c.getMonth())
		{
			return 1;
		}

		else
		{
			if(month < c.getMonth())
			{
				return -1;
			}

			if(day > c.getDay())
			{
				return 1;
			}

			else
			{
				if(day < c.getDay())
				{
					return -1;
				}

				else
				{
					return strcmp(charge, c.getCharge().c_str());
				}
			}
		}
	}
}

void CandidateMethods::print(const char* recordBytes, int recordSize)
{
	Candidate c(12, 12, 2000, "Invalid", "Invalid", 10101010);
	c.setBytes((char*)recordBytes);

	cout << "(" << c.getName() << " " << c.getCharge() << " " << c.getDni() << ")" << endl;
}

void CandidateMethods::printKey(const char* key, int recordSize)
{
	Candidate c(12, 12, 2000, "Invalid", "Invalid", 10101010);
	c.setBytes((char*)key);

	cout << "(" << c.getYear() << ", " << c.getMonth() << ", " << c.getDay() << ", " << c.getCharge() <<
			", " << c.getName() << ", " << c.getDni() << ")" << endl;
}

VariableRecord* CandidateMethods::getKeyRecord(const char* recordBytes, int recordSize)
{
	VariableRecord* record = new VariableRecord();
	record->setBytes(recordBytes, recordSize);

	return record;
}

CandidateMethods::~CandidateMethods()
{
}
