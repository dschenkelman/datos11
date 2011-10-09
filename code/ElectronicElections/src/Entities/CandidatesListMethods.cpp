/*
 * CandidatesListMethods.cpp
 *
 *  Created on: 09/10/2011
 *      Author: alejandro
 */

#include "CandidatesListMethods.h"
#include "../Entities/CandidatesList.h"
#include "../VariableBlocks/Constants.h"
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

CandidatesListMethods::CandidatesListMethods()
{
}

int CandidatesListMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
	CandidatesList c(1, 1, 1, "Invalid", "Invalid");
	c.setBytes((char*) recordBytes);

	if(strcmp(key, c.getKey()) == 0)
	{
		return 0;
	}

	int i = 0;

	short year;
	char day, month;
	memcpy(&year, key+i, sizeof(short)); i += sizeof(short);
	memcpy(&month, key+i, sizeof(char)); i += sizeof(char);
	memcpy(&day, key+i, sizeof(char)); i += sizeof(char);

	char len = (key+i)[0]; i += Constants::FIELD_HEADER_SIZE;
	char charge[len];
	memcpy(charge, key+i, len); i += len;

	len = (key+i)[0]; i += Constants::FIELD_HEADER_SIZE;
	char name[len];
	memcpy(name, key+i, len); i += len;

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
					int res = strcmp(charge, c.getCharge().c_str());
					if (res) return res;
					return strcmp(name, c.getListName().c_str());
				}
			}
		}
	}
}

void CandidatesListMethods::print(const char* recordBytes, int recordSize)
{
	CandidatesList c(1, 1, 1, "Invalid", "Invalid");
	c.setBytes((char*) recordBytes);

	cout << "(" << c.getYear() << ", " << (int) c.getMonth() << ", " << (int) c.getDay()<< ", " << c.getCharge() << ", " << c.getListName() << ")" << endl;
}

void CandidatesListMethods::printKey(const char* key, int recordSize)
{
	return print(key, recordSize); // INDICE=RECORD
}

VariableRecord* CandidatesListMethods::getKeyRecord(const char* recordBytes, int recordSize)
{
	VariableRecord* record = new VariableRecord();
	record->setBytes(recordBytes, recordSize);

	return record;
}

CandidatesListMethods::~CandidatesListMethods()
{
}

