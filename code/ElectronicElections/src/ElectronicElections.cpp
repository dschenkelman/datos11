//============================================================================
// Name        : ElectronicElections.cpp
// Author      : Datos11
// Version     :
// Copyright   : 
// Description : Trying block file out
//============================================================================

#include <iostream>
#include <stdlib.h>
#include "Customer.h"
#include "Blocks/BlockFile.h"
#include "CustomerMethods.h"
using namespace std;

int main()
{
	srand (time(NULL));

	char firstNames[5][5] = {"Mike", "John", "Tony", "Rick", "Josh"};
	char lastNames[5][10] = {"Connor", "Sparano", "Lewis", "Pittino", "Smith"};

	string f = "test";
	BlockFile* file = new BlockFile(f, 512, new CustomerMethods, true);

	for (long i = 0; i < 500; ++i)
	{
		int fn = rand() % 5;
		int ln = rand() % 5;

		Customer c;
		c.firstName = firstNames[fn];
		c.lastName = lastNames[ln];
		c.balance = i;

		int l1 = strlen(c.firstName) + 1;
		int l2 = strlen(c.lastName) + 1;
		// size includes 3 control fields of 4 bytes each
		int size = l1 + l2 + sizeof(long) + (4 * 3);

		char* recordKey = new char[l1 + l2 - 1];
		memset(recordKey, 0, l1 + l2 - 1);
		strcat(recordKey, c.firstName);
		strcat(recordKey, c.lastName);

		char* recordBytes = new char[size];
		memset(recordBytes, 0, size);

		memcpy(recordBytes, &l1, 4);
		memcpy(recordBytes + 4, c.firstName, l1);

		memcpy(recordBytes + 4 + l1, &l2, 4);
		memcpy(recordBytes + (8 + l1), c.lastName, l2);

		int balanceSize = sizeof(long);
		memcpy(recordBytes + (8 + l1 + l2), &balanceSize, 4);
		memcpy(recordBytes + (12 + l1 + l2), &c.balance, sizeof(long));

		file->insertRecord(recordKey, recordBytes, size);
		delete[] recordBytes;
		delete[] recordKey;
	}

	file->printContent();

	delete file;

	return 0;
}
