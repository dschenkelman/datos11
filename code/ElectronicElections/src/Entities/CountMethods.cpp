/*
 * CountMethods.cpp
 *
 *  Created on: 05/10/2011
 *      Author: lejosdelcielo
 */

#include "CountMethods.h"
#include "../Entities/Count.h"
#include <iostream>
#include <cstring>

using namespace std;

CountMethods::CountMethods()
{
}

int CountMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
	// falta implementar
	return 1;
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

	cout << "(" << c.getYear() << ", " << c.getMonth() << ", " << c.getDay() << ", " << c.getCharge() << ", "
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

