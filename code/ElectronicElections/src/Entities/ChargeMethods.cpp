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
	// falta implementar
	return 1;
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
	VariableRecord* record = new VariableRecord();
	record->setBytes(recordBytes, recordSize);

	return record;
}

ChargeMethods::~ChargeMethods()
{
}

