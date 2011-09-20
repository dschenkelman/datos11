
/*
 * TreeBlockFileTests.cpp
 *
 *  Created on: Sep 20, 2011
 *      Author: damian
 */

#include "TreeBlockFileTests.h"
#include "CustomerMethods.h"
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;

TreeBlockFileTests::TreeBlockFileTests()
{
	string f = "treetest";
	this->file = new TreeBlockFile(f, 512, 16, new CustomerMethods, true);
}

void TreeBlockFileTests::testInsert()
{
	std::cout << "==================================" << std::endl;
	std::cout << "Insert Test" << std::endl;
	std::cout << "==================================" << std::endl;

	char firstNames[5][5] = {"John", "Mike", "Tony", "Rick", "Josh"};
	char lastNames[5][7] = {"Connor", "Potter", "Wesley", "Mordor", "Gondor"};
	int blockNumber = 0;
	for(long i = 0;i < 500;++i)
	{
		int fn = rand() % 5;
		int ln = rand() % 5;
		Customer c;
		c.firstName = firstNames[fn];
		c.lastName = lastNames[ln];
		c.balance = i;
		int l1 = strlen(c.firstName) + 1;
		int l2 = strlen(c.lastName) + 1;
		// size = 16
		int size = l1 + l2 + sizeof (long);
		char *recordKey = new char[l1 + l2 - 1];
		memset(recordKey, 0, l1 + l2 - 1);
		strcat(recordKey, c.firstName);
		strcat(recordKey, c.lastName);
		char *recordBytes = new char[size];
		memcpy(recordBytes, c.firstName, l1);
		memcpy(recordBytes + l1, c.lastName, l2);
		memcpy(recordBytes + (l1+l2), &c.balance, sizeof(long));
		Block* block = file->getCurrentBlock();
		if (block->isFull())
		{
			file->saveBlock();
			blockNumber++;
			file->loadBlock(blockNumber);
			block = file->getCurrentBlock();
		}
		block->insertRecord(recordKey, recordBytes);
		delete [] recordBytes;
		delete [] recordKey;
	}
	file->saveBlock();
	file->printContent();
}

void TreeBlockFileTests::run()
{
	this->testInsert();
}


TreeBlockFileTests::~TreeBlockFileTests()
{
	delete this->file;
}
