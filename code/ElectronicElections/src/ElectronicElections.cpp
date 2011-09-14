//============================================================================
// Name        : ElectronicElections.cpp
// Author  : Datos11
// Version     :
// Copyright   : 
// Description : Trying block file out
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <string>
#include "Customer.h"
#include "Blocks/BlockFile.h"
#include "CustomerMethods.h"
using namespace std;

void testInsertion(char firstNames[5][5], char lastNames[5][10], BlockFile *& file)
{
	for(long i = 0;i < 500;++i){
		int fn = rand() % 5;
		int ln = rand() % 5;
		Customer c;
		c.firstName = firstNames[fn];
		c.lastName = lastNames[ln];
		c.balance = i;
		int l1 = strlen(c.firstName) + 1;
		int l2 = strlen(c.lastName) + 1;
		int size = l1 + l2 + sizeof (long ) + (4 * 3);
		char *recordKey = new char[l1 + l2 - 1];
		memset(recordKey, 0, l1 + l2 - 1);
		strcat(recordKey, c.firstName);
		strcat(recordKey, c.lastName);
		char *recordBytes = new char[size];
		memset(recordBytes, 0, size);
		memcpy(recordBytes, &l1, 4);
		memcpy(recordBytes + 4, c.firstName, l1);
		memcpy(recordBytes + 4 + l1, &l2, 4);
		memcpy(recordBytes + (8 + l1), c.lastName, l2);
		int balanceSize = sizeof (long );
		memcpy(recordBytes + (8 + l1 + l2), &balanceSize, 4);
		memcpy(recordBytes + (12 + l1 + l2), &c.balance, sizeof (long ));
		file->insertRecord(recordKey, recordBytes, size);
		delete [] recordBytes;
		delete [] recordKey;
	}
	file->printContent();
}

void testUpdate(BlockFile*& file)
{
	string key = "JohnConnor";
	long balance = 5000;

	string firstName = "John";
	int l1 = 5;

	string lastName = "Copperfield";
	int l2 = 12;

	int size = 5 + 12 + 4 + 4 * 3;
	char* buffer = new char[size];

	memcpy(buffer, &l1, 4);
	memcpy(buffer + 4, firstName.c_str(), l1);
	memcpy(buffer + 4 + l1, &l2, 4);
	memcpy(buffer + (8 + l1), lastName.c_str(), l2);
	int balanceSize = sizeof(long);
	memcpy(buffer + (8 + l1 + l2), &balanceSize, 4);
	memcpy(buffer + (12 + l1 + l2), &balance, balanceSize);
	file->updateRecord(key.c_str(), buffer, size);
	file->printContent();
}

int main()
{
	srand (time(NULL));

	char firstNames[5][5] = {"John", "Mike", "Tony", "Rick", "Josh"};
	char lastNames[5][10] = {"Connor", "Sparano", "Lewis", "Pittino", "Smith"};

	string f = "test";
	BlockFile* file = new BlockFile(f, 512, new CustomerMethods, true);

	// 25 customers should be printed
	testInsertion(firstNames, lastNames, file);

	// John Connor => John Copperfield 5000
	testUpdate(file);

	delete file;

	return 0;
}
