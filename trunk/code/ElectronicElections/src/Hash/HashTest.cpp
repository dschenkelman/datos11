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
	std::cout << "==================================" << std::endl;

	char firstNames[9][5] = {"John", "Mike", "Tony", "Rick", "Josh","gaby","dami", "juan","aleT"};
	char lastNames[5][7] = {"Connor", "Potter", "Wesley", "Mordor", "Gondor"};
	for(long i = 0;i < 70;++i)
	{
		int fn = rand() % 9;
		int ln = rand() % 5;
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
/*
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
	//gotRecord = this->hash->getRecord(recordKey, gotRecord);
	std::cout << gotRecord->getBytes() << endl;
	
	delete [] recordKey;
}*/

void HashTest::testRemove()
{
	std::cout << "==================================" << std::endl;
	std::cout << "Remove Hash " << std::endl;
	std::cout << "==================================" << std::endl;

	char firstNames[9][5] = {"John", "Mike", "Tony", "Rick", "Josh","gaby","dami", "juan","aleT"};
	char lastNames[5][7] = {"Connor", "Potter", "Wesley", "Mordor", "Gondor"};
	for(long i = 0;i < 200;++i)
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
	//this->testGetRecord();
	this->testRemove();
	this->testEmptyBlock(2);
	std::cout << "Remove Successful" << endl;
}

HashTest::~HashTest()
{
	delete this->file;
}
