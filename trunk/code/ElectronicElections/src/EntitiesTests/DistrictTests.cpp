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
	std::string name = "Rosario";
	District dis(name);

	char bytes[dis.getSize()];
	char len = name.size() + 1;
	memcpy(bytes, &len, Constants::FIELD_HEADER_SIZE);
	memcpy(bytes+Constants::FIELD_HEADER_SIZE, name.c_str(), len);

	if(strcmp(bytes, dis.getBytes()) != 0)
	{
		return false;
	}

	return true;
}

bool DistrictTests::testSetBytes()
{
	District dis("Rosario");

	char bytes[12]; //tam del registro cuando el nombre sea Corrientes
	std::string name = "Corrientes";
	char len = name.size() + 1; // por el /0
	memcpy(bytes, &len, Constants::FIELD_HEADER_SIZE);
	memcpy(bytes+Constants::FIELD_HEADER_SIZE, name.c_str(), len);

	dis.setBytes(bytes);

	if(strcmp((char*)name.c_str(), (char*)dis.getName().c_str()) != 0)
	{
		return false;
	}

	return true;
}

DistrictTests::~DistrictTests()
{
}

