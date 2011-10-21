/*
 * VoterMethods.cpp
 *
 *  Created on: 09/10/2011
 *      Author: lejosdelcielo
 */

#include "VoterMethods.h"
#include "Voter.h"
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

VoterMethods::VoterMethods()
{
}

int VoterMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
	std::vector<ElectionKey> list;
	Voter v(0, "Invalid", "1234", "Invalid", "Invalid", list);

	v.setBytes((char*) recordBytes);

	int dni;
	memcpy(&dni, key, sizeof(int));

	if (dni == v.getDni()) return 0;
	else if (dni > v.getDni()) return 1;
	else return -1;
}

void VoterMethods::print(const char* recordBytes, int recordSize)
{
	std::vector<ElectionKey> list;
	Voter v(0, "Invalid", "1234", "Invalid", "Invalid", list);

	v.setBytes((char*) recordBytes);

	cout << "(" << v.getDni() << ", " << v.getNames() << ", " << v.getAddress() << ", "
			<< v.getDistrict() << ")" << endl;
}

void VoterMethods::printKey(const char* key, int recordSize)
{
	int dni = atoi(key+1);

	cout << "(" << dni << ")" << endl;
}

VariableRecord* VoterMethods::getKeyRecord(const char* recordBytes, int recordSize)
{
	char dni[sizeof(int)];
	memcpy(dni, recordBytes, sizeof(int));
	VariableRecord* record = new VariableRecord();
	record->setBytes(dni, sizeof(int));

	return record;
}

VoterMethods::~VoterMethods()
{
}

