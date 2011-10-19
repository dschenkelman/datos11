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
	char bytes[recordSize];
	int i = 0;
	memcpy(bytes, (char*) key, recordSize - sizeof(unsigned int)); i += (recordSize - sizeof(unsigned int));
	unsigned int q = 1000;
	memcpy(bytes+i, &q, sizeof(unsigned int));

	Count c(1, 1, 1, "Invalid", "Invalid", "Invalid", 1);
	c.setBytes(bytes);

	cout << "(" << c.getYear() << ", " << (int) c.getMonth() << ", " << (int) c.getDay()<< ", " << c.getCharge() << ", "
			<< c.getListName() << ", " << c.getDistrict() << ")" << endl;
}

VariableRecord* CountMethods::getKeyRecord(const char* recordBytes, int recordSize)
{
	VariableRecord* record = new VariableRecord();
	record->setBytes(recordBytes, recordSize);

	return record;
}

CountMethods::~CountMethods()
{
}

