/*
 * SequenceTreeBlockTests.cpp
 *
 *  Created on: 08/10/2011
 *      Author: JuanMa
 */

#include "SequenceTreeBlockTests.h"
#include "../BPlusTree/SequenceTreeBlock.h"
#include "../Entities/District.h"
#include "../Entities/DistrictMethods.h"
#include "../VariableBlocks/VariableRecord.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

SequenceTreeBlockTests::SequenceTreeBlockTests()
{
	block1 = new SequenceTreeBlock(128,&districtMethods,true);
}


bool SequenceTreeBlockTests::testPopFirstWorksCorrectly()
{


	string districts[] = {"Devon", "Leicestershire", "Manchester", "Shropshire", "Warwickshire" };

	for (int i = 0; i < 5; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());

		block1->insertRecord(&keyRecord,&dataRecord);
	}

	this->print();

	for (int i = 0; i < 5; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		char *bytes = block1->popFirst()->getBytes();
		if ( strcmp(dataRecord.getBytes(), bytes) != 0)
		{
			return false;
		}
		this->print();
	}

	return true;
}
void SequenceTreeBlockTests::print()
{
	VariableRecord r;
	this->block1->positionAtBegin();
	cout << endl;
	while(this->block1->getNextRecord(&r) != NULL)
	{
		this->districtMethods.print(r.getBytes(), r.getSize());
	}
	cout << endl;
}


void SequenceTreeBlockTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n"));
}

void SequenceTreeBlockTests::run()
{
	printResult("testPopFirstWorksCorrectly", testPopFirstWorksCorrectly());
}

SequenceTreeBlockTests::~SequenceTreeBlockTests()
{
}

