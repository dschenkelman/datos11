/*
 * ElectionTests.cpp
 *
 *  Created on: 06/10/2011
 *      Author: lejosdelcielo
 */

#include "ElectionTests.h"
#include "../Entities/Election.h"
#include "../VariableBlocks/Constants.h"
#include <iostream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

ElectionTests::ElectionTests()
{
}

void ElectionTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n"));
}

void ElectionTests::run()
{
	printResult("testGetSize", testGetSize());
	printResult("testGetKeySize", testGetKeySize());
	printResult("testSetBytes", testSetBytes());
	printResult("testGetBytes", testGetBytes());
	printResult("testGetKey", testGetKey());
}

bool ElectionTests::testGetSize()
{
	short year = 2011;
	char month = 10;
	char day = 23;
	std::vector<std::string> districtList;
	std::string dOne = "Rosario";
	std::string dTwo = "Chubut";
	std::string charge = "Intendente";
	districtList.push_back(dOne); districtList.push_back(dTwo);

	Election e(year, month, day, charge, districtList);

	int size = sizeof(year) + sizeof(month) + sizeof(day) + Constants::FIELD_HEADER_SIZE + 1 + charge.size();
	size += Constants::FIELD_HEADER_SIZE;
	size += Constants::FIELD_HEADER_SIZE * 2 + 2 + dOne.size() + dTwo.size();

	if(size != e.getSize())
	{
		return false;
	}

	return true;
}

bool ElectionTests::testGetKeySize()
{
	short year = 2011;
	char month = 10;
	char day = 23;
	std::vector<std::string> districtList;
	std::string dOne = "Rosario";
	std::string dTwo = "Chubut";
	std::string charge = "Intendente";
	districtList.push_back(dOne); districtList.push_back(dTwo);

	Election e(year, month, day, charge, districtList);
	int size = sizeof(year) + sizeof(month) + sizeof(day) + Constants::FIELD_HEADER_SIZE + 1 + charge.size();

	if(size != e.getKeySize())
	{
		return false;
	}

	return true;
}

bool ElectionTests::testSetBytes()
{
	short year = 2011;
	char month = 10;
	char day = 23;
	std::string dOne = "Rosario";
	std::string dTwo = "Chubut";
	std::string charge = "Intendente";

	int size = sizeof(year) + sizeof(month) + sizeof(day);
	size += Constants::FIELD_HEADER_SIZE + 1 + charge.size();
	size += Constants::FIELD_HEADER_SIZE;
	size += Constants::FIELD_HEADER_SIZE * 2 + 2 + dOne.size() + dTwo.size();

	char bytes[size];
	int i = 0;

	memcpy(bytes+i, &year, sizeof(short)); i += sizeof(short);
	memcpy(bytes+i, &month, sizeof(char)); i += sizeof(char);
	memcpy(bytes+i, &day, sizeof(char)); i += sizeof(char);

	char len = charge.size() + 1;

	memcpy(bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, charge.c_str(), len); i += len;

	char listSize = 2;

	memcpy(bytes+i, &listSize, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;

	len = dOne.size() + 1;
	memcpy(bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, dOne.c_str(), len); i += len;

	len = dTwo.size() + 1;
	memcpy(bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, dTwo.c_str(), len); i += len;

	std::vector<std::string> districtList;

	Election e(2, 1, 1999, "Invalid", districtList);
	e.setBytes(bytes);

	if(e.getDay() != day)
	{
		return false;
	}

	if(e.getMonth() != month)
	{
		return false;
	}

	if(e.getYear() != year)
	{
		return false;
	}

	if(e.getCharge() != charge)
	{
		return false;
	}

	if(e.getDistrictList().at(0) != dOne)
	{
		return false;
	}

	if(e.getDistrictList().at(1) != dTwo)
	{
		return false;
	}

	return true;
}

bool ElectionTests::testGetBytes()
{
	short year = 2011;
	char month = 10;
	char day = 23;
	std::vector<std::string> districtList;
	std::string dOne = "Rosario";
	std::string dTwo = "Chubut";
	std::string charge = "Intendente";
	districtList.push_back(dOne); districtList.push_back(dTwo);

	Election e(year, month, day, charge, districtList);
	std::vector<std::string> invalidVec;

	Election eTwo(12, 12, 290, "Invalid", invalidVec);
	eTwo.setBytes(e.getBytes());

	if(e.getDay() != eTwo.getDay())
	{
		return false;
	}

	if(e.getMonth() != eTwo.getMonth())
	{
		return false;
	}

	if(e.getYear() != eTwo.getYear())
	{
		return false;
	}

	if(e.getCharge() != eTwo.getCharge())
	{
		return false;
	}

	if(e.getDistrictList().at(0) != eTwo.getDistrictList().at(0))
	{
		return false;
	}

	if(e.getDistrictList().at(1) != eTwo.getDistrictList().at(1))
	{
		return false;
	}

	return true;
}

bool ElectionTests::testGetKey()
{
	short year = 2011;
	char month = 10;
	char day = 23;
	std::string charge = "Intendente";

	int size = sizeof(year) + sizeof(month) + sizeof(day);
	size += Constants::FIELD_HEADER_SIZE + 1 + charge.size();

	char key[size];
	int i = 0;

	memcpy(key+i, &year, sizeof(short)); i += sizeof(short);
	memcpy(key+i, &month, sizeof(char)); i += sizeof(char);
	memcpy(key+i, &day, sizeof(char)); i += sizeof(char);

	char len = charge.size() + 1;

	memcpy(key+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(key+i, charge.c_str(), len); i += len;

	std::vector<std::string> districtList;

	Election e(day, month, year, charge, districtList);

	return strcmp(e.getKey(), key) == 0;
}

ElectionTests::~ElectionTests()
{
}

