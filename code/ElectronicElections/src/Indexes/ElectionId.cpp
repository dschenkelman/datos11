/*
 * ElectionId.cpp
 *
 *  Created on: Oct 16, 2011
 *      Author: damian
 */

#include "ElectionId.h"

ElectionId::ElectionId():day(0), month(0), year(0), charge("None")
{
}

ElectionId::ElectionId(char d, char m, short  y, std::string & c) : day(d), month(m), year(y), charge(c)
{
}

char ElectionId::getDay()
{
	return this->day;
}

std::string & ElectionId::getCharge()
{
	return this->charge;
}

char ElectionId::getMonth()
{
	return this->month;
}

short ElectionId::getYear()
{
	return this->year;
}

void ElectionId::setYear(short  value)
{
	this->year = value;
}

void ElectionId::setDay(char value)
{
	this->day = value;
}

void ElectionId::setMonth(char value)
{
	this->month = value;
}

void ElectionId::setCharge(std::string & value)
{
	this->charge = value;
}

ElectionId::~ElectionId()
{
}
