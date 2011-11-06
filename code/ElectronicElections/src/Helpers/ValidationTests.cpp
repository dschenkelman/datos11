/*
 * ValidationTests.cpp
 *
 *  Created on: 06/11/2011
 *      Author: lejosdelcielo
 */

#include "ValidationTests.h"
#include "Validation.h"

ValidationTests::ValidationTests()
{
}

void ValidationTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n"));
}

void ValidationTests::run()
{
	this->printResult("testValidateDate", testValidateDate());
}

bool ValidationTests::testValidateDate()
{
	Validation validation;

	short yearOne = 2000;
	char monthOne = 2;
	char dayOne = 29;

	if(validation.isValidDate(yearOne, monthOne, dayOne) != true)
	{
		return false;
	}

	short yearTwo = 3001;
	char monthTwo = 2;
	char dayTwo = 22;

	if(validation.isValidDate(yearTwo, monthTwo, dayTwo) != false)
	{
		return false;
	}

	short yearThree = 2012;
	char monthThree = 13;
	char dayThree = 12;

	if(validation.isValidDate(yearThree, monthThree, dayThree) != false)
	{
		return false;
	}

	short yearFour = 2012;
	char monthFour = 0;
	char dayFour = 12;

	if(validation.isValidDate(yearFour, monthFour, dayFour) != false)
	{
		return false;
	}

	short yearFive = 2012;
	char monthFive = 4;
	char dayFive = 0;

	if(validation.isValidDate(yearFive, monthFive, dayFive) != false)
	{
		return false;
	}

	short yearSix = 2012;
	char monthSix = 4;
	char daySix = 31;

	if(validation.isValidDate(yearSix, monthSix, daySix) != false)
	{
		return false;
	}

	return true;
}

ValidationTests::~ValidationTests()
{
}
