/*
 * RLVBlockFileTests.cpp
 *
 *  Created on: Sep 15, 2011
 *      Author: damian
 */

#include <stdlib.h>
#include "RLVBlockFileTests.h"
#include <string>
#include <iostream>
#include "Customer.h"
#include "CustomerMethods.h"
#include "./../Blocks/Constants.h"

using namespace std;

RLVBlockFileTests::RLVBlockFileTests()
{
	string f = "RLVtest";
	this->file = new RLVBlockFile(f, 512, new CustomerMethods, true);
}

void RLVBlockFileTests::testInsert()
{
	std::cout << "==================================" << std::endl;
	std::cout << "Insert RLV Test" << std::endl;
	std::cout << "==================================" << std::endl;

	int insertedRecords = 0;
	char firstNames[5][5] = {"John", "Mike", "Tony", "Rick", "Josh"};
	char lastNames[5][7] = {"Connor", "Potter", "Wesley", "Mordor", "Gondor"};
	for(long i = 0;i < 100;++i)
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
		if(file->insertRecord(recordKey, recordBytes, size) )
		{
			insertedRecords++;
		}

		//No need to handle Blocks or Records.
		//we should use the blockFile to insert with the most basic logic.
		/*RLVBlock* block = file->getCurrentBlock();
		if (! block->canInsertRecord(size) )
		{
			file->saveBlock();
			blockNumber++;
			file->loadBlock(blockNumber);
			block = file->getCurrentBlock();
		}
		VariableRecord* record = new VariableRecord();
		record->setBytes(recordBytes, size);
		block->insertRecord(recordKey, record);*/

		delete [] recordBytes;
		delete [] recordKey;
	}
	file->saveBlock();
	file->printContent();
	std::cout << "CountInserted: " << insertedRecords << std::endl;
}

void RLVBlockFileTests::testGet()
{
	string key = "JohnKratos";

	VariableRecord* rec = new VariableRecord();
	file->loadBlock(0);
	std::cout << "==================================" << std::endl;
	std::cout << "Get RLVTest" << std::endl;
	std::cout << "==================================" << std::endl;
	if (file->getRecord(key.c_str(), &rec))
	{
		CustomerMethods cm;
		cm.print(rec->getBytes(), rec->getSize());
	}
	else
	{
		std::cout << key << "--> was not found in file" << std::endl;
	}
	string trueKey = "JohnConnor";
	if (file->getRecord(trueKey.c_str(), &rec))
	{
		CustomerMethods cm;
		cm.print(rec->getBytes(), rec->getSize());
	}
	else
	{
		std::cout << trueKey << "--> was not found in file" << std::endl;
	}

}

void RLVBlockFileTests::testRemove()
{
	std::cout << "==================================" << std::endl;
	std::cout << "Remove Test" << std::endl;
	std::cout << "==================================" << std::endl;

	string key = "JohnConnor";
	VariableRecord* rec();
	file->loadBlock(0);
	std::cout << "Removing key: " << key << std::endl;
	if (file->removeRecord(key.c_str()))
	{
		file->saveBlock();
		std::cout << key << "--> key removed!" << std::endl;
		file->printContent();
	}
	else
	{
		std::cout << key << "--> couldn't remove key" << std::endl;
	}
}

void RLVBlockFileTests::testUpdate()
{
	/*std::cout << "==================================" << std::endl;
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
	file->printContent();*/
}

void RLVBlockFileTests::run()
{
	this->testInsert();
	//this->testUpdate();
	this->testGet();
	this->testRemove();
}

RLVBlockFileTests::~RLVBlockFileTests()
{
	delete this->file;
}
