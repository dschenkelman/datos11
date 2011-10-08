/*
 * HashTest.cpp
 *
 *  Created on: Sep 20, 2011
 *      Author: gabriel
 */

#include "HashTest.h"
#include "../BlocksTests/CustomerMethods.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include "../Entities/District.h"

using namespace std;

HashTest::HashTest()
{
	string f = "hashtest";
	int blockAmount = 10;
	this->file = new HashBlockFile(f, 512, new CustomerMethods, blockAmount, true);
}

void HashTest::testInsert()
{
	std::cout << "==================================" << std::endl;
	std::cout << "Insert Hash " << std::endl;

	char firstNames[10][5] = {"John", "Mike", "Tony", "Rick", "Josh", "gaby", "dami", "juan","aleT", "gonz"};
	char lastNames[9][7] = {"Connor", "Potter", "Wesley", "Mordor", "Gondor", "ostrow", "shenke", "Torrad", "Durand"};
	for(long i = 0;i < 100;++i)
	{
		int fn = rand() % 10;
		int ln = rand() % 9;
		Customer c;
		c.firstName = firstNames[fn];
		c.lastName = lastNames[ln];
		c.balance = i;
		int l1 = strlen(c.firstName) + 1;
		int l2 = strlen(c.lastName) + 1;
		// size = 16
		int size = l1+1 + l2+1 + sizeof (int); //sumo 2 bytes para cada tamaño del nombre y apellido
		char *recordKey = new char[l1 + l2 - 1];
		memset(recordKey, 0, l1 + l2 - 1);
		strcat(recordKey, c.firstName);
		strcat(recordKey, c.lastName);
		char *recordBytes = new char[size];
		memcpy(recordBytes, &l1, sizeof(char));
		memcpy(recordBytes + 1 , c.firstName, l1);
		memcpy(recordBytes +1+ l1, &l2, sizeof(char));
		memcpy(recordBytes +1+ l1 + 1, c.lastName, l2);
		memcpy(recordBytes +2+ (l1+l2), &c.balance, sizeof(int));
		
		//blockNumber = hasingName(recordKey);
		this->file->insertRecord(recordKey, recordBytes, size);
		
		delete [] recordBytes;
		delete [] recordKey;
	}
	this->file->printContent();
}

void HashTest::testGetRecord()
{
	std::cout << "==================================" << std::endl;
	std::cout << "Get Record from Hash.." << std::endl;

	CustomerMethods* cm;
	Customer c;
	c.firstName = "John"; //char= 5
	c.lastName = "Connor"; //char= 7
	c.balance = 2; //not relevant
	int keySize = 5 + 7 - 1;
	char *recordKey = new char[keySize];
	memset(recordKey, 0, keySize);
	strcat(recordKey, c.firstName);
	strcat(recordKey, c.lastName);
	
	VariableRecord* gotRecord = new VariableRecord();
	if( this->file->getRecord(recordKey, &gotRecord))
	{
		c = *(cm->getCustomerFromRecord(gotRecord->getBytes(), gotRecord->getSize()));
		std::cout << c.firstName << c.lastName << c.balance << endl;
	}

	else std::cout << "couldn't find key: " << recordKey << endl;
	delete [] recordKey;
	delete gotRecord;
	std::cout << "GetRecord Successful!" << endl;
	std::cout << "==================================" << std::endl;
}

void HashTest::testUpdateRecord()
{
	std::cout << "==================================" << std::endl;
	std::cout << "UPDATE JohnConnor from Hash.." << std::endl;

	CustomerMethods* cm;
	Customer c;
	c.firstName = "John"; //char= 5
	c.lastName = "Connor"; //char= 7
	c.balance = 2; //not relevant
	int keySize = 5 + 7 - 1;
	char *recordKey = new char[keySize];
	memset(recordKey, 0, keySize);
	strcat(recordKey, c.firstName);
	strcat(recordKey, c.lastName);
	VariableRecord* gotRecord = cm->getRecordFromCustomer(&c);
	if( this->file->updateRecord(recordKey, gotRecord))
		std::cout << "Record UPDATED!" << endl;
	else std::cout << "couldn't find or insert new key: " << recordKey << endl;

	delete [] recordKey;
	delete gotRecord;
	std::cout << "UPDATE Hash Successful!" << endl;
	std::cout << "==================================" << std::endl;
}

void HashTest::testRemove()
{
	std::cout << "==================================" << std::endl;
	std::cout << "REMOVE Hash.." << std::endl;

	char firstNames[9][5] = {"John", "Mike", "Tony", "Rick", "Josh","gaby","dami", "juan","aleT"};
	char lastNames[5][7] = {"Connor", "Potter", "Wesley", "Mordor", "Gondor"};
	for(long i = 0;i < 10;++i)
	{
		int fn = rand() % 9;
		int ln = rand() % 5;
		Customer c;
		c.firstName = firstNames[fn];
		c.lastName = lastNames[ln];
		c.balance = i;
		int keySize = 5 + 7 - 1;
		char *recordKey = new char[keySize];
		memset(recordKey, 0, keySize);
		strcat(recordKey, c.firstName);
		strcat(recordKey, c.lastName);
		VariableRecord* gotRecord = new VariableRecord();
		if(this->file->removeRecord(recordKey) )
			std::cout << "key: " << recordKey << endl;
		else std::cout << "couldn't remove key: " << recordKey << endl;

		delete [] recordKey;
		delete gotRecord;
	}
	this->file->printContent();
}

void HashTest::testEmptyBlock(int blockNumber)
{
	std::cout << "Empty hash block number 2?" << std::endl;
	this->file->loadBlock(blockNumber);
	if(this->file->getCurrentBlock()->isEmpty())
		std::cout << "YES!" << std::endl;
	else std::cout << "NOO" << std::endl;

	if(this->file->getCurrentBlock()->getOverflowedBlock() == -1)
		std::cout << "Block became DES-overflowed!! :D" << std::endl;
}

void HashTest::run()
{
	this->testEmptyBlock(2);
	this->testInsert();
	std::cout << "Inserted Hash successful" << std::endl;
	std::cout << "==================================" << std::endl;
	this->testGetRecord();
	this->testUpdateRecord();
	//this->testRemove();
	this->testEmptyBlock(2);
	std::cout << "Remove Successful" << endl;
	std::cout << "==================================" << std::endl;
}

HashTest::~HashTest()
{
	delete this->file;
}
