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
	memcpy(&nameLen, key+i, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	char name[nameLen];
	memcpy(&name, key+i, nameLen);

	int conditionOne = (year > list.getYear());
	int conditionTwo = (month > list.getMonth());
	int conditionThree = (day > list.getDay());
	int conditionFour = strcmp(charge, list.getCharge().c_str());
	int conditionFive = strcmp(name, list.getName().c_str());

	// no estoy nada seguro de que es lo que hay que devolver
	return conditionOne && conditionTwo && conditionThree && conditionFour && conditionFive;
}

void ElectionsListMethods::print(const char* recordBytes, int recordSize)
{
	ElectionsList list("Lista0", 22, 12, 2000, "Intendente");
	list.setBytes((char*)recordBytes);

	cout << "(" << list.getName() << " " << list.getCharge() << ")" << endl;
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

