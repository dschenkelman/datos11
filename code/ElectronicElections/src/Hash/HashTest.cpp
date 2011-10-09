/*
 * HashTest.cpp
 *
 *  Created on: Sep 20, 2011
 *      Author: gabriel
 */

#include "HashTest.h"
#include "../BlocksTests/CustomerMethods.h"
#include "../Entities/DistrictMethods.h"
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

void HashTest::testLoadHashwithoutValidation()
{
	std::cout << "==================================" << std::endl;
	std::cout << "Load Hash " << std::endl;
	HashBlockFile* districtHash = new HashBlockFile("districthash", 1024, new DistrictMethods, 1000, true);
	string districts[] = {"San Luis", "Santa Cruz", "Santa Fe", "Santiago del Estero",
			"Corrientes", "Tierra del Fuego", "Tucuman", "Entre Rios",
			"Chaco", "Chubut", "Cordoba",
			"Mendoza", "Misiones", "Neuquen", "Rio Negro", "Salta", "San Juan",
			"Buenos Aires", "Catamarca", "Formosa", "Jujuy", "La Pampa", "La Rioja",
			};
	VariableRecord dataRecord;
	VariableRecord keyRecord;
	for (int i = 0; i < 60000; i++)
	{
		District d(districts[i%23]);
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());

		districtHash->loadRecord(keyRecord.getBytes(), &dataRecord);
	}
	districtHash->printContent();
	std::cout << "Loaded District Hash successful" << std::endl;
	std::cout << "==================================" << std::endl;
	std::cout << "Update San Juan Hash.." << std::endl;
	District dUpdate("Barcelo");
	District dOld("San Luis");
	dataRecord.setBytes(dUpdate.getBytes(), dUpdate.getSize());
	districtHash->updateRecord(dOld.getKey(), &dataRecord);
	//districtHash->printContent();
	std::cout << "==================================" << std::endl;
	delete districtHash;
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
		char recordKey[l1 + l2 - 1];
		memset(recordKey, 0, l1 + l2 - 1);
		strcat(recordKey, c.firstName);
		strcat(recordKey, c.lastName);
		char recordBytes[size];
		memcpy(recordBytes, &l1, sizeof(char));
		memcpy(recordBytes + 1 , c.firstName, l1);
		memcpy(recordBytes +1+ l1, &l2, sizeof(char));
		memcpy(recordBytes +1+ l1 + 1, c.lastName, l2);
		memcpy(recordBytes +2+ (l1+l2), &c.balance, sizeof(int));
		VariableRecord record;
		record.setBytes(recordBytes, size);
		//blockNumber = hasingName(recordKey);
		this->file->insertRecord(recordKey, &record);
	}
	this->file->printContent();
	std::cout << "Inserted Hash successful" << std::endl;
	std::cout << "==================================" << std::endl;
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
	c.firstName = "dami"; //char= 5
	c.lastName = "shenke"; //char= 7
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

	char firstNames[10][5] = {"John", "Mike", "Tony", "Rick", "Josh","gaby","dami", "juan","aleT", "gonz"};
	char lastNames[9][7] = {"Connor", "Potter", "Wesley", "Mordor", "Gondor", "shenke", "ostrow", "Durand","Torrad"};
	for(long i = 0;i < 300;++i)
	{
		int fn = rand() % 10;
		int ln = rand() % 9;
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
	std::cout << "Remove Successful" << endl;
	std::cout << "==================================" << std::endl;
}

void HashTest::testEmptyBlock(int blockNumber)
{
	std::cout << "Empty hash block?";
	this->file->loadBlock(blockNumber);
	if(this->file->getCurrentBlock()->isEmpty())
		std::cout << "YES!" << std::endl;
	else std::cout << "NOO" << std::endl;
	if(this->file->getCurrentBlock()->getOverflowedBlock() == -1)
		std::cout << "Block became DES-overflowed!! :D" << std::endl;
}

void HashTest::run()
{
	//this->testLoadHashwithoutValidation();
	this->testInsert();
	this->testGetRecord();
	this->testUpdateRecord();
	this->testRemove();

}

HashTest::~HashTest()
{
	delete this->file;
}
