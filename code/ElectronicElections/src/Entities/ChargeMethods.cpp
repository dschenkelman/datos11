/*
 * ChargeMethods.cpp
 *
 *  Created on: 06/10/2011
 *      Author: lejosdelcielo
 */

#include "ChargeMethods.h"
#include "../Entities/Charge.h"
#include "../VariableBlocks/Constants.h"
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

ChargeMethods::ChargeMethods()
{
}

int ChargeMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
	std::vector<std::string> invalidVec;
	Charge c("Invalid", invalidVec);

	c.setBytes((char*) recordBytes);

	char len = key[0];
	char chargeAux[len];
	memcpy(chargeAux, key+1, len);

	return strcmp(chargeAux, c.getCharge().c_str());
}

void ChargeMethods::print(const char* recordBytes, int recordSize)
{
	std::vector<std::string> invalidVec;
	Charge c("Invalid", invalidVec);

	c.setBytes((char*) recordBytes);

	cout << "(" << c.getCharge();

	for(int i = 0; i < c.getChargeList().size(); i++)
	{
		cout << ", " << c.getChargeList().at(i);
	}

	cout << ")" << endl;
}

void ChargeMethods::printKey(const char* key, int recordSize)
{
	char len = key[0];
	char nameAux[len];
	memcpy(nameAux, key+Constants::FIELD_HEADER_SIZE, len);
	std::string keyStr = nameAux;

	cout << "(" << keyStr << ")" << endl;
}

VariableRecord* ChargeMethods::getKeyRecord(const char* recordBytes, int recordSize)
{
	char len = recordBytes[0];
	char nameAux[len];
	memcpy(nameAux, recordBytes+Constants::FIELD_HEADER_SIZE, len);
	VariableRecord* record = new VariableRecord();
	record->setBytes(nameAux, len);

	return record;
}

ChargeMethods::~ChargeMethods()
{
}

