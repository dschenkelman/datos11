/*
 * CountId.cpp
 *
 *  Created on: Oct 17, 2011
 *      Author: damian
 */

#include "CountId.h"

CountId::CountId()
: day(0), month(0), year(0), charge(""), listName("")
{
}


CountId::CountId(char d, char m, short y, std::string & c, std::string & lName)
: day(d), month(m), year(y), charge(c), listName(lName)
{
}

short CountId::getYear()
{
	return this->year;
}

char CountId::getMonth()
{
	return this->month;
}

std::string & CountId::getCharge()
{
	return this->charge;
}

void CountId::setListName(std::string & value)
{
	this->listName = value;
}


void CountId::setMonth(char value)
{
	this->month = value;
}

void CountId::setDay(char value)
{
	this->day = value;
}

void CountId::setYear(short  value)
{
	this->year = value;
}

void CountId::setCharge(std::string & value)
{
	this->charge = value;
}

std::string & CountId::getListName()
{
	return this->listName;
}

char CountId::getDay()
{
	return this->day;
}

CountId::~CountId()
{
}
