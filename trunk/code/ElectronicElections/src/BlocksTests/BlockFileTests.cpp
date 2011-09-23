/*
 * BlockFileTests.cpp
 *
 *  Created on: Sep 15, 2011
 *      Author: damian
 */

#include <stdlib.h>
#include "BlockFileTests.h"
#include <string>
#include <iostream>
#include "Customer.h"
#include "./../Hash/HashBlockFile.h"
#include "CustomerMethods.h"
#include "./../Blocks/Constants.h"

using namespace std;

HashBlockFileTests::HashBlockFileTests()
{
	string f = "test";
	this->file = new HashBlockFile(f, 512, 16, new CustomerMethods, true, 40);
}

void HashBlockFileTests::testInsert()
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

void HashBlockFileTests::testGet()
{
	string key = "JohnKratos";

	Record rec(16);
	file->loadBlock(0);
	std::cout << "==================================" << std::endl;
	std::cout << "Get Test" << std::endl;
	std::cout << "==================================" << std::endl;
	if (file->getCurrentBlock()->findRecord(key.c_str(), &rec) >= 0)
	{
		CustomerMethods cm;
		cm.print(rec.getBytes(), rec.getSize());
	}
}

void HashBlockFileTests::testRemove()
{
	string key = "JohnKratos";

	Record rec(16);
	file->loadBlock(0);
	std::cout << "==================================" << std::endl;
	std::cout << "Remove Test" << std::endl;
	std::cout << "==================================" << std::endl;
	while (file->getCurrentBlock()->removeRecord(key.c_str()))
	{
		file->saveBlock();
		file->printContent();
	}
}

void HashBlockFileTests::testUpdate()
{
	std::cout << "==================================" << std::endl;
	std::cout << "Update Test" << std::endl;
	std::cout << "==================================" << std::endl;

	string key1 = "JohnConnor";
	string key2 = "MikeGondor";
	long balance = 5000;

	string firstName = "John";
	int l1 = 5;

	string lastName = "Kratos";
	int l2 = 7;

	int size = 16;
	char buffer[size];

	memcpy(buffer, firstName.c_str(), l1);
	memcpy(buffer + l1, lastName.c_str(), l2);
	memcpy(buffer + (l1+l2), &balance, 4);

	file->loadBlock(0);
	file->getCurrentBlock()->updateRecord(key1.c_str(), buffer);
	file->getCurrentBlock()->updateRecord(key2.c_str(), buffer);
	file->saveBlock();
	file->printContent();
}

void HashBlockFileTests::run()
{
	this->testInsert();
	this->testUpdate();
	this->testGet();
	this->testRemove();
}

HashBlockFileTests::~HashBlockFileTests()
{
	delete this->file;
}
