/*
 * ElectionsListMethods.cpp
 *
 *  Created on: 04/10/2011
 *      Author: gonzalo
 */

#include "ElectionsListMethods.h"
#include "ElectionsList.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

ElectionsListMethods::ElectionsListMethods() {
	// TODO Auto-generated constructor stub

}

int ElectionsListMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
	ElectionsList list("Lista0", 22, 12, 2000, "Intendente");
	list.setBytes((char*)recordBytes);

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

	char nameLen = 0;
	memcpy(&nameLen, key+i, Constants::FIELD_HEADER_SIZE); i+= Constants::FIELD_HEADER_SIZE;
	char name[nameLen];
	memcpy(&name, key+i, nameLen); i += nameLen;

	if(year > list.getYear())
	{
		return 1;
	}

	else
	{
		if(year < list.getYear())
		{
			return -1;
		}

		if(month > list.getMonth())
		{
			return 1;
		}

		else
		{
			if(month < list.getMonth())
			{
				return -1;
			}

			if(day > list.getDay())
			{
				return 1;
			}

			else
			{
				if(day < list.getDay())
				{
					return -1;
				}

				else
				{
					if( strcmp(charge, list.getCharge().c_str()) != 0)
					{
						return strcmp(charge, list.getCharge().c_str());
					}
					else
					{
						return strcmp(name, list.getName().c_str());
					}
				}
			}
		}
	}
}

void ElectionsListMethods::print(const char* recordBytes, int recordSize)
{
	ElectionsList list("Lista0", 22, 12, 2000, "Intendente");
	list.setBytes((char*)recordBytes);

	cout << "(" << list.getName() << ", " << list.getCharge()<< ", " << (short)list.getDay() << "/" << (short)list.getMonth() << "/" << list.getYear() << ")" << endl;
}

void ElectionsListMethods::printKey(const char* key, int recordSize)
{
	ElectionsList list("Lista0", 22, 12, 2000, "Intendente");
	list.setBytes((char*)key);

	cout << "(" << list.getYear() << " " << list.getMonth() << " " << list.getDay() <<
			" " << list.getCharge() << " " << list.getName() << ")" <<  endl;
}

VariableRecord* ElectionsListMethods::getKeyRecord(const char* recordBytes, int recordSize)
{
	VariableRecord* record = new VariableRecord();
	record->setBytes(recordBytes, recordSize);

	return record;
}

ElectionsListMethods::~ElectionsListMethods() {
	// TODO Auto-generated destructor stub
}

