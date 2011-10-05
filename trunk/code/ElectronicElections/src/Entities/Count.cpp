/*
 * Count.cpp
 *
 *  Created on: 05/10/2011
 *      Author: lejosdelcielo
 */

#include "Count.h"
#include "../VariableBlocks/Constants.h"
#include <string>

Count::Count(char day, char month, short year, std::string charge, std::string name, std::string district, unsigned int quantity)
{
	this->day = day;
	this->month = month;
	this->year = year;
	this->charge = charge;
	this->name = name;
	this->district = district;
	this->quantity = quantity;
	this->bytes = NULL;
}

int Count::getSize()
{
	int size = 0;
	size += sizeof(this->year) + sizeof(this->month) + sizeof(this->day);
	size += Constants::FIELD_HEADER_SIZE + this->charge.size() + 1;
	size += Constants::FIELD_HEADER_SIZE + this->name.size() + 1;
	size += Constants::FIELD_HEADER_SIZE + this->name.size() + 1;
	size += sizeof(this->quantity);

	return size;
}

Count::~Count() {
	// TODO Auto-generated destructor stub
}

