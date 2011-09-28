/*
 * DistrictTests.cpp
 *
 *  Created on: 28/09/2011
 *      Author: gonzalo
 */

#include "DistrictTests.h"
#include "../Entities/District.h"

DistrictTests::DistrictTests()
{
}

void DistrictTests::run()
{
	printResult("testGetSizeReturnsCorrectValue", testGetSizeReturnsCorrectValue());
	//printResult("testGetBytesReturnsCorrectValue", testGetBytesReturnsCorrectValue());
	//printResult("testSetBytes", testSetBytes());
}

void DistrictTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n"));
}

bool DistrictTests::testGetSizeReturnsCorrectValue()
{
	District dis("Rosario");

	if(dis.getSize() != 9)
	{
		return false;
	}

	else
	{
		return true;
	}
}

bool DistrictTests::testGetBytesReturnsCorrectValue()
{
	District dis("Rosario");

	char bytes[dis.getSize()];
	int size = 8;
	memcpy(bytes, &size, 1);
	char* name = "Rosario";
	memcpy(bytes+1, &name, size);

	if(strcmp(bytes, dis.getBytes()) != 0)
	{
		return false;
	}

	else
	{
		return true;
	}
}

bool DistrictTests::testSetBytes()
{
	District dis("Rosario");

	char bytes[12];
	int size = 11;
	memcpy(bytes, &size, 1);
	char* name = "Corrientes";
	memcpy(bytes+1, &name, size);

	dis.setBytes(bytes);

	cout << dis.getName() << " " << bytes << endl;

	if(strcmp(bytes, (char*)dis.getName().c_str()) != 0)
	{
		return false;
	}

	else
	{
		return true;
	}
}

DistrictTests::~DistrictTests()
{
}

