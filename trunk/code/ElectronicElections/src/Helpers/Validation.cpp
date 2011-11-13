/*
 * Validation.cpp
 *
 *  Created on: 06/11/2011
 *      Author: lejosdelcielo
 */

#include "Validation.h"
#include <string>

using namespace std;

Validation::Validation()
{
}

bool Validation::isValidDate(short year, char month, char day)
{
	if(year > 3000 || year < 0)
	{
		return false;
	}

	if(this->isValidMonth(month))
	{
		if(this->isLeapYear(year))
		{
			if (month == 2 && day == 29)
			{
				return true;
			}
		}

		return this->isValidDay(day, month);
	}

	return false;
}

bool Validation::isValidDay(char day, char month)
{
	if(day < 1)
	{
		return false;
	}

	char longMonths[] = {1, 3, 5, 7, 8, 10, 12};
	int longMonthsSize = 7;
	char shortMonths[] = {4, 6, 9, 11};
	int shortMonthsSize = 4;

	if(this->isInArray(month, shortMonths, shortMonthsSize))
	{
		if(day > 30)
		{
			return false;
		}

		return true;
	}

	if(this->isInArray(month, longMonths, longMonthsSize))
	{
		if(day > 31)
		{
			return false;
		}

		return true;
	}

	if(month == 2)
	{
		if (day > 28)
		{
			return false;
		}

		return true;
	}

	return false;
}

bool Validation::isInArray(char month, char* months, int arraySize)
{
	for(int i = 0; i < arraySize; i++)
	{
		if (months[i] == month)
		{
			return true;
		}
	}

	return false;
}

bool Validation::isValidMonth(char month)
{
	bool conditionOne = month < 13;
	bool conditionTwo = month > 0;

	return conditionOne && conditionTwo;
}

bool Validation::isLeapYear(short year)
{
	bool conditionOne = (year % 4 == 0);
	bool conditionTwo = (year % 100 == 0);
	bool conditionThree = (year % 400 == 0);

	return conditionOne && (!conditionTwo || conditionThree);
}

Validation::~Validation()
{
}

