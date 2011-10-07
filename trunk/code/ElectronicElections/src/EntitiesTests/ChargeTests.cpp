/*
 * ChargeTests.cpp
 *
 *  Created on: 06/10/2011
 *      Author: lejosdelcielo
 */

#include "ChargeTests.h"
#include "../Entities/Charge.h"
#include "../VariableBlocks/Constants.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <iostream>

using namespace std;

ChargeTests::ChargeTests()
{
}

void ChargeTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n"));
}

void ChargeTests::run()
{
	printResult("testGetSize", testGetSize());
	printResult("testGetKeySize", testGetKeySize());
	printResult("testSetBytes", testSetBytes());
	printResult("testGetBytes", testGetBytes());
	printResult("testGetKey", testGetKey());
}

bool ChargeTests::testGetSize()
{
	std::string charge = "Intendente";
	std::string chargeTwo = "Gobernador";
	std::string chargeThree = "Presidente";
	std::vector<std::string> vector;
	vector.push_back(chargeTwo);
	vector.push_back(chargeThree);

	Charge c(charge, vector);

	int size = Constants::FIELD_HEADER_SIZE * 3 + 3 + charge.size() + chargeTwo.size() + chargeThree.size();

	if(c.getSize() != size)
	{
		return false;
	}

	return true;
}

bool ChargeTests::testGetKeySize()
{
	std::string charge = "Intendente";
	std::string chargeTwo = "Gobernador";
	std::string chargeThree = "Presidente";
	std::vector<std::string> vector;
	vector.push_back(chargeTwo);
	vector.push_back(chargeThree);

	Charge c(charge, vector);

	int size = Constants::FIELD_HEADER_SIZE + 1 + charge.size();

	if(size != c.getKeySize())
	{
		return false;
	}

	return true;
}

bool ChargeTests::testSetBytes()
{
	std::string charge = "Intendente";
	std::string chargeTwo = "Gobernador";
	std::string chargeThree = "Presidente";

	int size = charge.size() + chargeTwo.size() + chargeThree.size() + Constants::FIELD_HEADER_SIZE * 3 + 3;
	char bytes[size];

	int i = 0;
	char len = charge.size() + 1;

	memcpy(bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, charge.c_str(), len); i += len;

	char listSize = 2;
	memcpy(bytes+i, &listSize, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;

	len = chargeTwo.size() + 1;
	memcpy(bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, chargeTwo.c_str(), len); i += len;

	len = chargeThree.size() + 1;
	memcpy(bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, chargeThree.c_str(), len); i += len;

	std::vector<std::string> invalidVector;
	Charge c("Invalid", invalidVector);
	c.setBytes(bytes);

	if(c.getCharge() != charge)
	{
		return false;
	}

	if(c.getChargeList().size() != 2)
	{
		return false;
	}

	if(c.getChargeList().at(0) != chargeTwo)
	{
		return false;
	}

	if(c.getChargeList().at(1) != chargeThree)
	{
		return false;
	}

	return true;
}

bool ChargeTests::testGetBytes()
{
	std::string charge = "Intendente";
	std::string chargeTwo = "Gobernador";
	std::string chargeThree = "Presidente";
	std::vector<std::string> vector;
	vector.push_back(chargeTwo);
	vector.push_back(chargeThree);

	Charge c(charge, vector);
	std::vector<std::string> vec;
	Charge cTwo("Invalid", vec);

	cTwo.setBytes(c.getBytes());

	if(c.getCharge() != cTwo.getCharge())
	{
		return false;
	}

	if(c.getChargeList().size() != cTwo.getChargeList().size())
	{
		return false;
	}

	if(c.getChargeList().at(0) != cTwo.getChargeList().at(0))
	{
		return false;
	}

	if(c.getChargeList().at(1) != cTwo.getChargeList().at(1))
	{
		return false;
	}

	return true;
}

bool ChargeTests::testGetKey()
{
	std::string charge = "Intendente";
	std::string chargeTwo = "Gobernador";
	std::string chargeThree = "Presidente";
	std::vector<std::string> vector;
	vector.push_back(chargeTwo);
	vector.push_back(chargeThree);

	Charge c(charge, vector);

	char key[c.getKeySize()];
	int i = 0;

	char len = 1 + charge.size();
	memcpy(key+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(key+i, charge.c_str(), len);

	return strcmp(key, c.getKey()) == 0;
}

ChargeTests::~ChargeTests()
{
}

