/*
 * DistrictElections.cpp
 *
 *  Created on: Oct 16, 2011
 *      Author: damian
 */

#include "DistrictElections.h"
#include "../VariableBlocks/Constants.h"
#include <string.h>

using namespace std;

DistrictElections::DistrictElections():district("None")
{
	this->bytes = NULL;
}

DistrictElections::DistrictElections(std::string & d) : district(d)
{
	this->bytes = NULL;
}

void DistrictElections::addElection(char d, char m, short  y, std::string & c)
{
	ElectionId e(d, m, y,c);
	this->elections.push_back(e);
}

void DistrictElections::setDistrict(std::string & value)
{
	this->district = value;
}

std::string & DistrictElections::getDistrict()
{
	return this->district;
}

void DistrictElections::setBytes(const char *value)
{
	this->elections.clear();
	int position = 0;
	char districtLength = 0;

	memcpy(&districtLength, value, Constants::FIELD_HEADER_SIZE);
	position += Constants::FIELD_HEADER_SIZE;
	char dis[districtLength];
	memcpy(dis, value + position, districtLength);
	this->district = dis;
	position += districtLength;

	int electionsCount = 0;
	memcpy(&electionsCount, value + position, sizeof(int));
	position += sizeof(int);

	for(int i = 0; i < electionsCount; i++)
	{
		char d = 0;
		char m = 0;
		short y = 0;
		char chargeSize = 0;

		memcpy(&d, value + position, sizeof(char));
		position += sizeof(char);

		memcpy(&m, value + position, sizeof(char));
		position += sizeof(char);

		memcpy(&y, value + position, sizeof(short));
		position += sizeof(short);

		memcpy(&chargeSize, value + position, Constants::FIELD_HEADER_SIZE);
		position += Constants::FIELD_HEADER_SIZE;

		char chargeBuffer[chargeSize];
		memcpy(chargeBuffer, value + position, chargeSize);

		string charge = chargeBuffer;
		ElectionId e(d, m, y, charge);
		this->elections.push_back(e);
	}
}

char* DistrictElections::getBytes()
{
	int electionsCount = this->elections.size();
	char districtLength = this->district.length() + 1;

	int size = this->getSize();
	if (this->bytes != NULL)
	{
		delete this->bytes;
	}

	this->bytes = new char[size];

	int position = 0;

	memcpy(this->bytes, &districtLength, Constants::FIELD_HEADER_SIZE);
	position += Constants::FIELD_HEADER_SIZE;

	memcpy(this->bytes + position, this->district.c_str(), districtLength);
	position += districtLength;

	memcpy(this->bytes + position, &electionsCount, sizeof(int));
	position += sizeof(int);

	for(unsigned int i = 0; i < this->elections.size(); i++)
	{
		ElectionId e = this->elections.at(i);

		char d = e.getDay();
		char m = e.getMonth();
		short y = e.getYear();
		memcpy(this->bytes + position, &d, sizeof(char));
		position += sizeof(char);
		memcpy(this->bytes + position, &m, sizeof(char));
		position += sizeof(char);
		memcpy(this->bytes + position, &y, sizeof(short));
		position += sizeof(short);
		char chargeSize = e.getCharge().length() + 1;
		memcpy(this->bytes + position, &chargeSize, Constants::FIELD_HEADER_SIZE);
		position += Constants::FIELD_HEADER_SIZE;
		const char* charge = e.getCharge().c_str();
		memcpy(this->bytes + position, &charge, chargeSize);
	}

	return this->bytes;
}

int DistrictElections::getSize()
{
	int size = 0;
	size += Constants::FIELD_HEADER_SIZE;
	size += this->district.length() + 1;
	size += sizeof(int); //elections count

	for(unsigned int i = 0; i < this->elections.size(); i++)
	{
		size += 2 * sizeof(char) + sizeof(short); //day, month, year
		ElectionId e = this->elections.at(i);
		size += Constants::FIELD_HEADER_SIZE;
		size += e.getCharge().length() + 1;
	}

	return size;
}

DistrictElections::~DistrictElections()
{
	if (this->bytes != NULL)
	{
		delete this->bytes;
	}
}
