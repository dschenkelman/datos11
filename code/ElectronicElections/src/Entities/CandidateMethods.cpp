/*
 * CandidateMethods.cpp
 *
 *  Created on: 04/10/2011
 *      Author: lejosdelcielo
 */

#include "CandidateMethods.h"
#include "Candidate.h"
#include <iostream>

using namespace std;

CandidateMethods::CandidateMethods()
{
}

int CandidateMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
	//falta implementar
	return 1;
}

void CandidateMethods::print(const char* recordBytes, int recordSize)
{
	Candidate c(12, 12, 2000, "Invalid", "Invalid", 10101010);
	c.setBytes((char*)recordBytes);

	cout << "(" << c.getName() << " " << c.getCharge() << " " << c.getDni() << ")" << endl;
}

void CandidateMethods::printKey(const char* key, int recordSize)
{
	Candidate c(12, 12, 2000, "Invalid", "Invalid", 10101010);
	c.setBytes((char*)key);

	cout << "(" << c.getYear() << ", " << c.getMonth() << ", " << c.getDay() << ", " << c.getCharge() <<
			", " << c.getName() << ", " << c.getDni() << ")" << endl;
}

VariableRecord* CandidateMethods::getKeyRecord(const char* recordBytes, int recordSize)
{
	VariableRecord* record = new VariableRecord();
	record->setBytes(recordBytes, recordSize);

	return record;
}

CandidateMethods::~CandidateMethods()
{
}
