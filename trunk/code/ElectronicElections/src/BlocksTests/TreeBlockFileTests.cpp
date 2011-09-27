
/*
 * TreeBlockFileTests.cpp
 *
 *  Created on: Sep 20, 2011
 *      Author: damian
 */

#include "TreeBlockFileTests.h"
#include "CustomerMethods.h"
#include <string>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include "../VariableBlocks/VariableRecord.h"
#include "../VariableBlocks/Constants.h"

using namespace std;

TreeBlockFileTests::TreeBlockFileTests()
{
	string f = "treetest";
	this->file = new TreeBlockFile(f, 512, new CustomerMethods, true);
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
		// size = 7 + 5 + ...
		short size = l1 + l2 + sizeof (long) + 2 * Constants::FIELD_HEADER_SIZE;
		char recordKey[l1 + l2 - 1];
		memset(recordKey, 0, l1 + l2 - 1);
		strcat(recordKey, c.firstName);
		strcat(recordKey, c.lastName);
		char recordBytes[size];
		memcpy(recordBytes, &l1, Constants::FIELD_HEADER_SIZE);
		memcpy(recordBytes + Constants::FIELD_HEADER_SIZE, c.firstName, l1);
		memcpy(recordBytes + Constants::FIELD_HEADER_SIZE + l1, &l2, Constants::FIELD_HEADER_SIZE);
		memcpy(recordBytes + 2 * Constants::FIELD_HEADER_SIZE + l1, c.lastName, l2);
		memcpy(recordBytes + 2 * Constants::FIELD_HEADER_SIZE + (l1+l2), &c.balance, sizeof(long));
		TreeBlock* block = file->getCurrentBlock();
//		if (block->isFull())
//		{
//			file->saveBlock();
//			blockNumber++;
//			file->loadBlock(blockNumber);
//			block = file->getCurrentBlock();
//		}

		VariableRecord r;
		r.setBytes(recordBytes, size);
		block->insertRecord(recordKey, &r);
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
