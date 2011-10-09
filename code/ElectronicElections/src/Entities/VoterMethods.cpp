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

using namespace std;

VoterMethods::VoterMethods()
{
}

int VoterMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
	std::vector<ElectionKey> list;
	Voter v(11111111, "Fernando Gago", "1234", "Cordoba 900", "Neuquen", list);

	v.setBytes((char*) recordBytes);

	int dni = atoi(key+1);

	if(dni > v.getDni())
	{
		return 1;
	}

	else if(dni < v.getDni())
	{
		return -1;
	}

	else return 0;
}

void VoterMethods::print(const char* recordBytes, int recordSize)
{
	std::vector<ElectionKey> list;
	Voter v(11111111, "Fernando Gago", "1234", "Cordoba 900", "Neuquen", list);

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
	VariableRecord* record = new VariableRecord();
	record->setBytes(recordBytes, recordSize);

	return record;
}

VoterMethods::~VoterMethods()
{
}

