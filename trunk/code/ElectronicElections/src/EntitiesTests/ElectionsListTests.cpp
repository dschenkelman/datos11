/*
 * ElectionsListTests.cpp
 *
 *  Created on: 30/09/2011
 *      Author: lejosdelcielo
 */

#include "ElectionsListTests.h"
#include "../Entities/ElectionsList.h"
#include <iostream>

ElectionsListTests::ElectionsListTests()
{
}

void ElectionsListTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n"));
}

void ElectionsListTests::run()
{
	printResult("testGetSize", testGetSize());
	printResult("testGetBytes", testGetBytes());
}

bool ElectionsListTests::testGetSize()
{
	std::string name = "Lista0";
	std::string charge = "Intendente";
	ElectionsList list(name, 22, 06, 1986, charge);

	int res = (name.size() + 1 + Constants::FIELD_HEADER_SIZE) + (charge.size() + 1 + Constants::FIELD_HEADER_SIZE)
			+ sizeof(char) * 2 + sizeof(short);

	if (list.getSize() != res)
	{
		return false;
	}

	return true;
}

bool ElectionsListTests::testGetBytes()
{
	std::string name = "Lista19";
	std::string charge = "Intendente";
	ElectionsList list(name, 18, 04, 2009, charge);

	char bytes[27];

	int i = 0, size = 25;
	memcpy(bytes, &size, Constants::RECORD_HEADER_SIZE); i += Constants::RECORD_HEADER_SIZE;

	short year = 2009;
	memcpy(bytes+i, &year, sizeof(short)); i += sizeof(short);
	char month = 04;
	memcpy(bytes+i, &month, sizeof(char)); i += sizeof(char);
	char day = 18;
	memcpy(bytes+i, &day, sizeof(char)); i += sizeof(char);

	char chargeLen = charge.size() + 1;
	memcpy(bytes+i, &chargeLen, Constants::FIELD_HEADER_SIZE); i+= Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, charge.c_str(), chargeLen); i+= chargeLen;

	char nameLen = name.size() + 1;
	memcpy(bytes+i, &nameLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, name.c_str(), nameLen);

	if(strcmp(bytes, list.getBytes()) != 0)
	{
		return false;
	}

	return true;
}

bool ElectionsListTests::testSetBytes()
{
	std::string name = "Lista19";
	std::string charge = "Intendente";

	char bytes[27];

	int i = 0, size = 25;
	memcpy(bytes, &size, Constants::RECORD_HEADER_SIZE); i += Constants::RECORD_HEADER_SIZE;

	short year = 2009;
	memcpy(bytes+i, &year, sizeof(short)); i += sizeof(short);
	char month = 04;
	memcpy(bytes+i, &month, sizeof(char)); i += sizeof(char);
	char day = 18;
	memcpy(bytes+i, &day, sizeof(char)); i += sizeof(char);

	char chargeLen = charge.size() + 1;
	memcpy(bytes+i, &chargeLen, Constants::FIELD_HEADER_SIZE); i+= Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, charge.c_str(), chargeLen); i+= chargeLen;

	char nameLen = name.size() + 1;
	memcpy(bytes+i, &nameLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, name.c_str(), nameLen);

	ElectionsList electionsList("Lista1", 11, 11, 2000, "Gobernador");
	electionsList.setBytes(bytes);

	return true;
}

ElectionsListTests::~ElectionsListTests()
{
}

