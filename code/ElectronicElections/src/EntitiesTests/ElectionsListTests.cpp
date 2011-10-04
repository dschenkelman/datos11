/*
 * ElectionsListTests.cpp
 *
 *  Created on: 30/09/2011
 *      Author: lejosdelcielo
 */

#include "ElectionsListTests.h"
#include "../Entities/ElectionsList.h"
#include <iostream>

using namespace std;

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
	printResult("testSetBytes", testSetBytes());
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

	char bytes[25];

	int i = 0;

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

	ElectionsList listTwo("L20", 11, 11, 2003, "Alcalde");
	listTwo.setBytes(list.getBytes());

	if(list.getDay() != listTwo.getDay())
	{
		return false;
	}

	if(list.getMonth() != listTwo.getMonth())
	{
		return false;
	}

	if(list.getYear() != listTwo.getYear())
	{
		return false;
	}

	if(list.getName() != listTwo.getName())
	{
		return false;
	}

	if(list.getCharge() != listTwo.getCharge())
	{
		return false;
	}

	return true;
}

bool ElectionsListTests::testSetBytes()
{
	std::string name = "Lista19";
	std::string charge = "Intendente";

	char bytes[25];

	int i = 0;

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

	if(electionsList.getDay() != day)
	{
		return false;
	}

	if(electionsList.getMonth() != month)
	{
		return false;
	}

	if(electionsList.getYear() != year)
	{
		return false;
	}

	if(electionsList.getName() != name)
	{
		return false;
	}

	if(electionsList.getCharge() != charge)
	{
		return false;
	}

	return true;
}

ElectionsListTests::~ElectionsListTests()
{
}

