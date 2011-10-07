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

using namespace std;

ElectionMethods::ElectionMethods()
{
}

int ElectionMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
	// falta implementar
	return 1;
}

void ElectionMethods::print(const char* recordBytes, int recordSize)
{
	std::vector<std::string> vector;
	Election e(1 ,1 ,1990, "Gobernador", vector);

	e.setBytes((char*) recordBytes);

	cout << "(" << e.getDay() << "/" << e.getMonth() << "/" << e.getYear() << ", " << e.getCharge() << ")" << endl;
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

	cout << "(" << day << "/" << month << "/" << year << ", " << charge << ")" << endl;
}

VariableRecord* ElectionMethods::getKeyRecord(const char* recordBytes, int recordSize)
{
	VariableRecord* record = new VariableRecord();
	record->setBytes(recordBytes, recordSize);

	return record;
}

ElectionMethods::~ElectionMethods()
{
}

