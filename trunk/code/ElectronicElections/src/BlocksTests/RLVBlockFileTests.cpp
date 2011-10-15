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
#include "./../VariableBlocks/Constants.h"

using namespace std;

SimpleVariableBlockFileTests::SimpleVariableBlockFileTests()
{
	string f = "RLVtest";
	this->file = new SimpleVariableBlockFile(f, 512, new CustomerMethods, true);
}

void SimpleVariableBlockFileTests::testInsert()
{
	std::cout << "==================================" << std::endl;
	std::cout << "Insert RLV Test" << std::endl;
	std::cout << "==================================" << std::endl;

	int insertedRecords = 0;
	char firstNames[10][5] = {"John", "Mike", "Tony", "Rick", "Josh",
			"AleT", "Juan", "Gaby", "Dami", "Gonz"};
	char lastNames[5][7] = {"Connor", "Potter", "Wesley", "Mordor", "Gondor"};
	for(long i = 0;i < 100;++i)
	{
		int fn = rand() % 10;
		int ln = rand() % 5;
		Customer c;
		c.firstName = firstNames[fn];
		c.lastName = lastNames[ln];
		c.balance = i;
		int l1 = strlen(c.firstName) + 1;
		int l2 = strlen(c.lastName) + 1;
		// size = 16
		int size = l1+1 + l2+1 + sizeof (long); //sumo 2 bytes para cada tamaño del nombre y apellido
		char *recordKey = new char[l1 + l2 - 1];
		memset(recordKey, 0, l1 + l2 - 1);
		strcat(recordKey, c.firstName);
		strcat(recordKey, c.lastName);
		char *recordBytes = new char[size];
		memcpy(recordBytes, &l1, sizeof(char));
		memcpy(recordBytes + 1 , c.firstName, l1);
		memcpy(recordBytes +1+ l1, &l2, sizeof(char));
		memcpy(recordBytes +1+ l1 + 1, c.lastName, l2);

		memcpy(recordBytes +2+ (l1+l2), &c.balance, sizeof(long));
		if(file->insertRecord(recordKey, recordBytes, size) )
		{
			insertedRecords++;
		}

		//No need to handle Blocks or Records.
		//we should use the blockFile to insert with the most basic logic.

		delete [] recordBytes;
		delete [] recordKey;
	}
	file->saveBlock();
	file->printContent();
	std::cout << "CountInserted: " << insertedRecords << std::endl;
}

void SimpleVariableBlockFileTests::testGet()
{
	string key = "JohnKratos";

	VariableRecord* rec = NULL;
	file->loadBlock(0);
	std::cout << "==================================" << std::endl;
	std::cout << "Get RLVTest" << std::endl;
	std::cout << "==================================" << std::endl;
	if (file->getRecord(key.c_str(), &rec))
	{
		CustomerMethods cm;
		cm.print(rec->getBytes(), rec->getSize());
		if(rec != NULL)
		{
		delete rec;
		}
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

void SimpleVariableBlockFileTests::testRemove()
{
	std::cout << "==================================" << std::endl;
	std::cout << "Remove Test" << std::endl;
	std::cout << "==================================" << std::endl;

	string key = "JohnConnor";
	VariableRecord* rec();
	std::cout << "Removing key: " << key << std::endl;
	if (file->removeRecord(key.c_str()))
	{
		file->saveBlock();
		std::cout << key << "--> key removed!" << std::endl;
	}
	else
	{
		std::cout << key << "--> couldn't remove key" << std::endl;
	}
	string key2 = "GabyConnor";
	std::cout << "Removing key: " << key2 << std::endl;
	if (file->removeRecord(key2.c_str()))
	{
		file->saveBlock();
		std::cout << key2 << "--> key removed!" << std::endl;
	}
	else
	{
		std::cout << key2 << "--> couldn't remove key" << std::endl;
	}
	file->printContent();
}

void SimpleVariableBlockFileTests::testUpdate()
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
	int size = 18; //2 bytes for names lenght and 4 bytes for reg size
	char buffer[size];

	memcpy(buffer, &l1, sizeof(char));
	memcpy(buffer +1, firstName.c_str(), l1);
	memcpy(buffer + l1+1, &l2, sizeof(char));
	memcpy(buffer + l1+2, lastName.c_str(), l2);
	memcpy(buffer + (l1+l2)+2, &balance, 4);

	std::cout << "--Replacing " << key1 << " and " << key2 << "with John Kratos.."<< std::endl;
	file->updateRecord(key1.c_str(), buffer, size);
	file->updateRecord(key2.c_str(), buffer, size);
	file->saveBlock();
	file->printContent();
	std::cout << "NOTE: keys remains as JohnConnor & MikeGondor!" << std::endl;
}

void SimpleVariableBlockFileTests::run()
{
	int firstEmptyBlock = this->file->getFirstFreeEmptyBlock();
	std::cout << "First Free Block: " << firstEmptyBlock << endl;
	this->testInsert();
	firstEmptyBlock = this->file->getFirstFreeEmptyBlock();
	std::cout << "First Free Block: " << firstEmptyBlock << endl;
	this->testUpdate();
	this->testGet();
	this->testRemove();
	firstEmptyBlock = this->file->getFirstFreeEmptyBlock();
	std::cout << "First Free Block: " << firstEmptyBlock << endl;
}

SimpleVariableBlockFileTests::~SimpleVariableBlockFileTests()
{
	delete this->file;
}
