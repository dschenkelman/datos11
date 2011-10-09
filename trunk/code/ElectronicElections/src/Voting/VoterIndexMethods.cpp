/*
 * VoterIndexRecordMethods.cpp
 *
 *  Created on: Sep 23, 2011
 *      Author: damian
 */

#include "VoterIndexMethods.h"
#include <string.h>
#include <iostream>

VoterIndexMethods::VoterIndexMethods()
{
}

void VoterIndexMethods::print(const char *recordBytes, int recordSize)
{
	VoterIndex voter = this->getVoterIndexFromRecord(recordBytes, recordSize);
	std::cout << "(" << voter.DNI << ")";
}

VoterIndex VoterIndexMethods::getVoterIndexFromRecord(const char *recordBytes,
		int recordSize)
{
	VoterIndex voter;
	memcpy(&voter, recordBytes, recordSize);
	return voter;
}

int VoterIndexMethods::compare(const char *key,
		const char *recordBytes, int recordSize)
{
	VoterIndex voter = this->getVoterIndexFromRecord(recordBytes, recordSize);
	int keyDni;
	memcpy(&keyDni, key, sizeof(int));

	return keyDni - voter.DNI;
}

void VoterIndexMethods::printKey(const char* key, int recordSize)
{

}

VariableRecord *VoterIndexMethods::getKeyRecord(const char *recordBytes, int recordSize)
{
	return NULL;
}

VoterIndexMethods::~VoterIndexMethods()
{
}
