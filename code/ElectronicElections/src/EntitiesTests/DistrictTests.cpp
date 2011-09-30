/*
 * DistrictTests.cpp
 *
 *  Created on: 28/09/2011
 *      Author: gonzalo
 */

#include "DistrictTests.h"
#include "../Entities/District.h"
#include "../VariableBlocks/Constants.h"

DistrictTests::DistrictTests()
{
}

void DistrictTests::run()
{
	printResult("testGetSizeReturnsCorrectValue", testGetSizeReturnsCorrectValue());
	printResult("testGetBytesReturnsCorrectValue", testGetBytesReturnsCorrectValue());
	printResult("testSetBytes", testSetBytes());
}

void DistrictTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n"));
}

bool DistrictTests::testGetSizeReturnsCorrectValue()
{
	District dis("Rosario");

	if(dis.getSize() != 10)
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
	std::string name = "Rosario";
	District dis(name);

	char bytes[dis.getSize()];
	short size = 8;
	memcpy(bytes, &size, Constants::RECORD_HEADER_SIZE);
	memcpy(bytes+Constants::RECORD_HEADER_SIZE, name.c_str(), size);

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

	char bytes[13];
	short size = 11;
	memcpy(bytes, &size, Constants::RECORD_HEADER_SIZE);
	std::string name = "Corrientes";
	memcpy(bytes+Constants::RECORD_HEADER_SIZE, name.c_str(), size);

	dis.setBytes(bytes);

	if(strcmp((char*)name.c_str(), (char*)dis.getName().c_str()) != 0)
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

