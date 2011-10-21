/*
 * CountMethods.cpp
 *
 *  Created on: 05/10/2011
 *      Author: lejosdelcielo
 */

#include "CountMethods.h"
#include "../Entities/Count.h"
#include "../VariableBlocks/Constants.h"
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

CountMethods::CountMethods()
{
}

int CountMethods::compareKey(const char* key, const char* recordKey, int recordSize)
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

	// name
	len = (key+i)[0]; i += Constants::FIELD_HEADER_SIZE;
	char name[len];
	memcpy(name, key+i, len); i+= len;

	recordLen = (recordKey+j)[0]; j += Constants::FIELD_HEADER_SIZE;
	char recordName[recordLen];
	memcpy(recordName, recordKey+j, recordLen); j+= recordLen;

	//district
	len = (key+i)[0]; i += Constants::FIELD_HEADER_SIZE;
	char district[len];
	memcpy(district, key+i, len);

	recordLen = (recordKey+j)[0]; j += Constants::FIELD_HEADER_SIZE;
	char recordDistrict[recordLen];
	memcpy(recordDistrict, recordKey+j, recordLen);

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

				else
				{
					int chargeCompareResult = strcmp(charge, recordCharge);

					if(chargeCompareResult == 0)
					{
						int nameCmpResult = strcmp(name, recordName);
						if (nameCmpResult == 0)
						{
							return strcmp(district, recordDistrict);
						}

						return nameCmpResult;
					}

					else return chargeCompareResult;
				}
			}
		}
	}
}

int CountMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
	Count c(1, 1, 1, "Invalid", "Invalid", "Invalid", 0);
	c.setBytes((char*) recordBytes);

	int i = 0;

	short year;
	char day, month;
	memcpy(&year, key+i, sizeof(short)); i += sizeof(short);
	memcpy(&month, key+i, sizeof(char)); i += sizeof(char);
	memcpy(&day, key+i, sizeof(char)); i += sizeof(char);

	// charge
	char len = (key+i)[0]; i += Constants::FIELD_HEADER_SIZE;
	char charge[len];
	memcpy(charge, key+i, len); i+= len;

	// name
	len = (key+i)[0]; i += Constants::FIELD_HEADER_SIZE;
	char name[len];
	memcpy(name, key+i, len); i+= len;

	len = (key+i)[0]; i += Constants::FIELD_HEADER_SIZE;
	char district[len];
	memcpy(district, key+i, len);

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
					int chargeCompareResult = strcmp(charge, c.getCharge().c_str());

					if(chargeCompareResult == 0)
					{
						int nameCmpResult = strcmp(name, c.getListName().c_str());
						if (nameCmpResult == 0)
						{
							return strcmp(district, c.getDistrict().c_str());
						}

						return nameCmpResult;
					}

					else return chargeCompareResult;
				}
			}
		}
	}
}

void CountMethods::print(const char* recordBytes, int recordSize)
{
	Count c(1, 1, 1, "Invalid", "Invalid", "Invalid", 1);
	c.setBytes((char*) recordBytes);

	cout << "(" << c.getCharge() << ", " << c.getDistrict() << ", " << c.getListName() << ", " << c.getQuantity() << ")" << endl;
}

void CountMethods::printKey(const char* key, int recordSize)
{
	char bytes[recordSize + 4];
	int i = 0;
	memcpy(bytes, (char*) key, recordSize); i += recordSize;
	unsigned int q = 1000;
	memcpy(bytes+i, &q, sizeof(int));

	Count c(1, 1, 1, "Invalid", "Invalid", "Invalid", 1);
	c.setBytes(bytes);

	cout << "(" << c.getYear() << ", " << (int) c.getMonth() << ", " << (int) c.getDay()<< ", " << c.getCharge() << ", "
			<< c.getListName() << ", " << c.getDistrict() << ")" << endl;
}

VariableRecord* CountMethods::getKeyRecord(const char* recordBytes, int recordSize)
{
	VariableRecord* record = new VariableRecord();
	int keySize = recordSize - sizeof(int);
	char bytes[keySize];
	memset(bytes, 0, keySize);
	memcpy(bytes, recordBytes, keySize);

	record->setBytes(bytes, keySize);
	return record;
}

CountMethods::~CountMethods()
{
}

