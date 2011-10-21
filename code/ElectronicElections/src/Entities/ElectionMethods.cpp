/*
 * ElectionMethods.cpp
 *
 *  Created on: 07/10/2011
 *      Author: lejosdelcielo
 */

#include "ElectionMethods.h"
#include <string>
#include <vector>
#include "../Entities/Election.h"
#include <iostream>
#include <cstring>
#include "../VariableBlocks/Constants.h"

using namespace std;

ElectionMethods::ElectionMethods()
{
}

int ElectionMethods::compareKey(const char* key, const char* recordKey, int recordSize)
{
	int i = 0;
	int j = 0;
	short year, recordYear;
	char day, recordDay, month, recordMonth;

	memcpy(&year, key+i, sizeof(short));
	i += sizeof(short);
	memcpy(&recordYear, recordKey+j, sizeof(short));
	j += sizeof(short);

	memcpy(&month, key+i, sizeof(char));
	i += sizeof(char);
	memcpy(&recordMonth, recordKey+j, sizeof(char));
	j+= sizeof(char);

	memcpy(&day, key+i, sizeof(char));
	i += sizeof(char);
	memcpy(&recordDay, recordKey+j, sizeof(char));
	j+= sizeof(char);

	// charge
	char len = (key+i)[0];
	i += Constants::FIELD_HEADER_SIZE;
	char charge[len];
	memcpy(charge, key+i, len);
	i+= len;

	char recordLen = (recordKey+j)[0];
	j+= Constants::FIELD_HEADER_SIZE;
	char recordCharge[recordLen];
	memcpy(recordCharge, recordKey+j, recordLen);
	j+= recordLen;

	if(year > recordYear)
	{
		return 1;
	}

	else
	{
		if(year < recordYear)
		{
			return -1;
		}

		if(month > recordMonth)
		{
			return 1;
		}

		else
		{
			if(month < recordMonth)
			{
				return -1;
			}

			if(day > recordDay)
			{
				return 1;
			}

			else
			{
				if(day < recordDay)
				{
					return -1;
				}

				return strcmp(charge, recordCharge);
			}
		}
	}
}

int ElectionMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
	std::vector<std::string> vector;
	Election e(1 ,1 ,1990, "Gobernador", vector);
	e.setBytes((char*) recordBytes);

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

	if(year > e.getYear())
	{
		return 1;
	}

	else
	{
		if(year < e.getYear())
		{
			return -1;
		}

		if(month > e.getMonth())
		{
			return 1;
		}

		else
		{
			if(month < e.getMonth())
			{
				return -1;
			}

			if(day > e.getDay())
			{
				return 1;
			}

			else
			{
				if(day < e.getDay())
				{
					return -1;
				}

				else
				{
					return strcmp(charge, e.getCharge().c_str());
				}
			}
		}
	}
}

void ElectionMethods::print(const char* recordBytes, int recordSize)
{
	std::vector<std::string> vector;
	Election e(1 ,1 ,1990, "Gobernador", vector);

	e.setBytes((char*) recordBytes);

	cout << "(" << (short) e.getDay() << "/" << (short) e.getMonth() << "/" << e.getYear() << ", "
			<< e.getCharge() << ",(";

	for(int i = 0; i < e.getDistrictList().size(); i++)
	{
		cout << e.getDistrictList().at(i) << ",";
	}

	cout << "))";
}

void ElectionMethods::printKey(const char* key, int recordSize)
{
	int i = 0;
	short year;
	char day;
	char month;

	memcpy(&(year), key+i, sizeof(short)); i += sizeof(short);
	month = (key+i)[0]; i += sizeof(char);
	day = (key+i)[0]; i += sizeof(char);

	char len = (key+i)[0]; i+= sizeof(char);
	char chargeAux[len];
	memcpy(chargeAux, key+i, len); i += len;
	std::string charge = "";
	charge.append(chargeAux);

	cout << (short)day << "/" << (short)month << "/" << year << ", " << charge;
}

VariableRecord* ElectionMethods::getKeyRecord(const char* recordBytes, int recordSize)
{
	int keySize = 2 * sizeof(char) + sizeof(short);
	char chargeLen = 0;
	memcpy(&chargeLen, recordBytes + keySize, Constants::FIELD_HEADER_SIZE);
	keySize += Constants::FIELD_HEADER_SIZE + chargeLen;

	char buffer[keySize];
	memcpy(buffer, recordBytes, keySize);

	VariableRecord* record = new VariableRecord();
	record->setBytes(buffer, keySize);

	return record;
}

ElectionMethods::~ElectionMethods()
{
}

