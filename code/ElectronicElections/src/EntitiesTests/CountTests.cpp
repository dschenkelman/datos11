/*
 * CountTests.cpp
 *
 *  Created on: 05/10/2011
 *      Author: lejosdelcielo
 */

#include "CountTests.h"
#include "../Entities/Count.h"
#include "../VariableBlocks/Constants.h"

#include <iostream>

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

CountTests::~CountTests()
{
}

