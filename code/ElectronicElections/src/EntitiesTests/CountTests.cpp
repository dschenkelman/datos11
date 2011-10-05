/*
 * CountTests.cpp
 *
 *  Created on: 05/10/2011
 *      Author: lejosdelcielo
 */

#include "CountTests.h"
#include "../Entities/Count.h"
#include "../VariableBlocks/Constants.h"
#include "../Entities/CountMethods.h"

#include <iostream>
#include <cstring>

using namespace std;

CountTests::CountTests()
{
}

void CountTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n"));
}

void CountTests::run()
{
	printResult("testGetSize", testGetSize());
	printResult("testGetKeySize", testGetKeySize());
	printResult("testGetKey", testGetKey());
	printResult("testSetBytes", testSetBytes());
	printResult("testGetBytes", testGetBytes());
}

bool CountTests::testGetSize()
{
	std::string charge = "Gobernador";
	std::string list = "Lista19";
	std::string district = "Rosario";
	Count c(9, 1, 1990, charge, list, district, 150);

	int size = sizeof(char) * 2 + sizeof(short) + sizeof(unsigned int);
	size += Constants::FIELD_HEADER_SIZE + charge.size() + 1;
	size += Constants::FIELD_HEADER_SIZE + list.size() + 1;
	size += Constants::FIELD_HEADER_SIZE + district.size() +1;

	return c.getSize() == size;
}

bool CountTests::testGetKeySize()
{
	std::string charge = "Gobernador";
	std::string list = "Lista19";
	std::string district = "Rosario";
	Count c(9, 1, 1990, charge, list, district, 150);

	int size = sizeof(char) * 2 + sizeof(short);
	size += Constants::FIELD_HEADER_SIZE + charge.size() + 1;
	size += Constants::FIELD_HEADER_SIZE + list.size() + 1;
	size += Constants::FIELD_HEADER_SIZE + district.size() +1;

	return c.getKeySize() == size;
}

bool CountTests::testGetKey()
{
	char day = 9;
	char month = 1;
	short year = 1990;
	std::string charge = "Gobernador";
	std::string list = "Lista19";
	std::string district = "Rosario";
	Count c(day, month, year, charge, list, district, 150);

	char key[c.getKeySize()];
	int i = 0;

	memcpy(key+i, &year, sizeof(short)); i += sizeof(short);
	memcpy(key+i, &month, sizeof(char)); i += sizeof(char);
	memcpy(key+i, &day, sizeof(char)); i += sizeof(char);

	char chargeLen = charge.size() + 1;
	memcpy(key+i, &chargeLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(key+i, charge.c_str(), chargeLen); i += chargeLen;

	char listLen = list.size() + 1;
	memcpy(key+i, &listLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(key+i, list.c_str(), listLen); i += listLen;

	char districtLen = district.size() + 1;
	memcpy(key+i, &districtLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(key+i, district.c_str(), districtLen);

	return strcmp(key, c.getKey()) == 0;
}

bool CountTests::testSetBytes()
{
	char day = 9;
	char month = 1;
	short year = 1990;
	std::string charge = "Gobernador";
	std::string list = "Lista19";
	std::string district = "Rosario";
	unsigned int quantity = 1000;

	Count c(1, 11, 1999, "Invalid", "Invalid", "Invalid", 666);

	int size = sizeof(day) + sizeof(month) + sizeof(year) + (Constants::FIELD_HEADER_SIZE * 3) + charge.size() +
			list.size() + district.size() + sizeof(quantity) + 3; //3 por los /0 de cada string

	char bytes[size];
	int i = 0;

	memcpy(bytes+i, &year, sizeof(short)); i += sizeof(short);
	memcpy(bytes+i, &month, sizeof(char)); i += sizeof(char);
	memcpy(bytes+i, &day, sizeof(char)); i += sizeof(char);

	char chargeLen = charge.size() + 1;
	memcpy(bytes+i, &chargeLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, charge.c_str(), chargeLen); i += chargeLen;

	char listLen = list.size() + 1;
	memcpy(bytes+i, &listLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, list.c_str(), listLen); i += listLen;

	char districtLen = district.size() + 1;
	memcpy(bytes+i, &districtLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, district.c_str(), districtLen); i += districtLen;
	memcpy(bytes+i, &quantity, sizeof(unsigned int));

	c.setBytes(bytes);

	if(c.getDay() != day)
	{
		return false;
	}

	if(c.getMonth() != month)
	{
		return false;
	}

	if(c.getYear() != year)
	{
		return false;
	}

	if(c.getCharge() != charge)
	{
		return false;
	}

	if(c.getDistrict() != district)
	{
		return false;
	}

	if(c.getListName() != list)
	{
		return false;
	}

	if(c.getQuantity() != quantity)
	{
		return false;
	}

	return true;
}

bool CountTests::testGetBytes()
{
	char day = 9;
	char month = 1;
	short year = 1990;
	std::string charge = "Gobernador";
	std::string list = "Lista19";
	std::string district = "Rosario";
	unsigned int quantity = 1000;

	Count c(day, month, year, charge, list, district, quantity);
	Count cTwo(1, 11, 1999, "Invalid", "Invalid", "Invalid", 666);

	cTwo.setBytes(c.getBytes());

	if(c.getDay() != cTwo.getDay())
	{
		return false;
	}

	if(c.getMonth() != cTwo.getMonth())
	{
		return false;
	}

	if(c.getYear() != cTwo.getYear())
	{
		return false;
	}

	if(c.getCharge() != cTwo.getCharge())
	{
		return false;
	}

	if(c.getListName() != cTwo.getListName())
	{
		return false;
	}

	if(c.getDistrict() != cTwo.getDistrict())
	{
		return false;
	}

	if(c.getQuantity() != cTwo.getQuantity())
	{
		return false;
	}

	return true;
}

CountTests::~CountTests()
{
}

