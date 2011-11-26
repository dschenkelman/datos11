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

int VoterMethods::compareKey(const char* key, const char* recordKey, int recordSize)
{
	int dni, recordDni;
	memcpy(&dni, key, sizeof(int));
	memcpy(&recordDni, recordKey, sizeof(int));

	if (dni == recordDni) return 0;
	else if (dni > recordDni) return 1;
	else return -1;
}

int VoterMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
	std::vector<ElectionKey> list;
	std::vector<char> testvecPass;
	string testPass = "1234";
	for(int i=0; i< testPass.size(); i++)
	{
		testvecPass.push_back(testPass.at(i));
	}
	Voter v(0, "Invalid", testvecPass, "Invalid", "Invalid", list);

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
	std::vector<char> testvecPass;
	string testPass = "1234";
	for(int i=0; i< testPass.size(); i++)
	{
		testvecPass.push_back(testPass.at(i));
	}
	Voter v(0, "Invalid", testvecPass, "Invalid", "Invalid", list);

	v.setBytes((char*) recordBytes);

//	cout << "(" << v.getDni() << ", " << v.getNames() << ", " << v.getAddress() << ", "
//			<< v.getDistrict() << ")" << endl;
	cout << "DNI: " << v.getDni() << endl;
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

