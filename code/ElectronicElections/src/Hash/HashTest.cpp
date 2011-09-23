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
#include "../Hash/HashBlockFile.h"

using namespace std;

HashTest::HashTest()
{
	string f = "hashtest";
	int blockAmount = 20;
	this->file = new HashBlockFile(f, 512, 16, new CustomerMethods, true, blockAmount);
	this->hash = new Hash(file, blockAmount);
}

void HashTest::testInsert()
{
	std::cout << "==================================" << std::endl;
	std::cout << "Insert Hash " << std::endl;
	std::cout << "==================================" << std::endl;

	char firstNames[5][5] = {"John", "Mike", "Tony", "Rick", "Josh"};
	char lastNames[5][7] = {"Connor", "Potter", "Wesley", "Mordor", "Gondor"};
	int blockNumber;
	for(long i = 0;i < 5;++i)
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
		
		//blockNumber = hasingName(recordKey);
		blockNumber = atoi(recordKey) % 5;
		Record* record = new Record(16);
		record->setBytes(recordBytes);
		this->hash->insertRecord(recordKey, record);
		
		delete [] recordBytes;
		delete [] recordKey;
		delete [] record;
	}
	this->file->printContent();
}

void HashTest::testGetRecord()
{
	Customer c;
	c.firstName = "John"; //char= 5
	c.lastName = "Connor"; //char= 7
	c.balance = 2; //not relevant
	int keySize = 5 + 7 - 1;
	char *recordKey = new char[keySize];
	memset(recordKey, 0, keySize);
	strcat(recordKey, c.firstName);
	strcat(recordKey, c.lastName);
	
	Record* gotRecord = new Record(16);
	//*gotRecord = this->hash->getRecord(recordKey, gotRecord);
	std::cout << gotRecord->getBytes() << endl;
	
	delete [] recordKey;
}

void HashTest::testRemove()
{
	Customer c;
	c.firstName = "Mike"; //char= 5
	c.lastName = "Wesley"; //char= 7
	c.balance = 3; //not relevant
	int keySize = 5 + 7 - 1;
	char *recordKey = new char[keySize];
	memset(recordKey, 0, keySize);
	strcat(recordKey, c.firstName);
	strcat(recordKey, c.lastName);
	
	Record* gotRecord = new Record(16);
	//if( this->hash->getRecord(recordKey, gotRecord) != NULL)
	{
		//this->hash->removeRecord(recordKey);
	}
	std::cout << gotRecord->getBytes() << endl;
	
	delete [] recordKey;
}

void HashTest::run()
{
	this->testInsert();
	this->testGetRecord();
	this->testRemove();
	std::cout << "Remove Successful" << endl;
}

HashTest::~HashTest()
{
	delete this->hash;
	delete this->file;
}
