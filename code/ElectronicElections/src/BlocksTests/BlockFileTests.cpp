/*
 * BlockFileTests.cpp
 *
 *  Created on: Sep 15, 2011
 *      Author: damian
 */

#include <stdlib.h>
#include "BlockFileTests.h"
#include <string>
#include "Customer.h"
#include "./../Blocks/BlockFile.h"
#include "CustomerMethods.h"
#include "./../Blocks/Constants.h"

using namespace std;

BlockFileTests::BlockFileTests()
{
	string f = "test";
	this->file = new BlockFile(f, 512, 16, new CustomerMethods, true);
	string fileremoving = "removeTest";
	this->removeFile = new BlockFile(fileremoving, 512, 16, new CustomerMethods, true);
}

void BlockFileTests::testInsert()
{
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

void BlockFileTests::testGet()
{
//	string key = "JohnCopperfield";
//
//	Record* rec = NULL;
//	if (file->getRecord(key.c_str(), &rec))
//	{
//		CustomerMethods cm;
//		cm.print(rec->getBytes(), rec->getSize());
//	}
//
//
//	delete rec;
}

void BlockFileTests::testRemove()
{
//	char firstNames[5][5] = {"John", "Mike", "Tony", "Rick", "Josh"};
//	char lastNames[5][10] = {"Connor", "Sparano", "Lewis", "Pittino", "Smith"};
//
//	for(long i = 0;i < 5;++i)
//	{
//		Customer c;
//		c.firstName = firstNames[i];
//		c.lastName = lastNames[i];
//		c.balance = i;
//		int l1 = strlen(c.firstName) + 1;
//		int l2 = strlen(c.lastName) + 1;
//		int size = l1 + l2 + sizeof (long ) + (Constants::FIELD_HEADER_SIZE * 3);
//		char *recordKey = new char[l1 + l2 - 1];
//		memset(recordKey, 0, l1 + l2 - 1);
//		strcat(recordKey, c.firstName);
//		strcat(recordKey, c.lastName);
//		char *recordBytes = new char[size];
//		memset(recordBytes, 0, size);
//		memcpy(recordBytes, &l1, 4);
//		memcpy(recordBytes + Constants::FIELD_HEADER_SIZE, c.firstName, l1);
//		memcpy(recordBytes + Constants::FIELD_HEADER_SIZE + l1, &l2, Constants::FIELD_HEADER_SIZE);
//		memcpy(recordBytes + (Constants::FIELD_HEADER_SIZE * 2 + l1), c.lastName, l2);
//		int balanceSize = sizeof (long );
//		memcpy(recordBytes + (Constants::FIELD_HEADER_SIZE * 2 + l1 + l2), &balanceSize, 4);
//		memcpy(recordBytes + (Constants::FIELD_HEADER_SIZE * 3 + l1 + l2), &c.balance, sizeof (long ));
//		removeFile->insertRecord(recordKey, recordBytes, size);
//		delete [] recordBytes;
//		delete [] recordKey;
//	}
//
//	string key = "JohnConnor";
//	string firstName = "John";
//	string lastName = "Connor";
//	removeFile->removeRecord(key.c_str());
//	removeFile->printContent();
}

void BlockFileTests::testUpdate()
{
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

void BlockFileTests::run()
{
	this->testInsert();
	this->testUpdate();
	this->testGet();
	this->testRemove();
}

BlockFileTests::~BlockFileTests()
{
	delete this->file;
	delete this->removeFile;
}
