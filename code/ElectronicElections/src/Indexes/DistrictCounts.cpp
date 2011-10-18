/*
 * DistrictCounts.cpp
 *
 *  Created on: Oct 17, 2011
 *      Author: damian
 */

#include "DistrictCounts.h"
#include "../VariableBlocks/Constants.h"
#include <string.h>

using namespace std;

DistrictCounts::DistrictCounts():district("None")
{
	this->bytes = NULL;
	this->key = NULL;
}

DistrictCounts::DistrictCounts(std::string & d):district(d)
{
	this->bytes = NULL;
	this->key = NULL;
}


void DistrictCounts::setDistrict(std::string & value)
{
	this->district = value;
}

int DistrictCounts::getSize()
{
	int size = 0;
	size += Constants::FIELD_HEADER_SIZE;
	size += this->district.length() + 1;
	size += sizeof(int); //counts count

	for(unsigned int i = 0; i < this->counts.size(); i++)
	{
		size += 2 * sizeof(char) + sizeof(short); //day, month, year
		CountId c = this->counts.at(i);
		size += Constants::FIELD_HEADER_SIZE;
		size += c.getCharge().length() + 1;
		size += Constants::FIELD_HEADER_SIZE;
		size += c.getListName().length() + 1;
	}

	return size;
}

char *DistrictCounts::getKey()
{
	char districtLength = this->district.length() + 1;
	int keyLength = this->district.length() + 1 + Constants::FIELD_HEADER_SIZE;
	if (this->key != NULL)
	{
		delete this->key;
	}

	this->key = new char[keyLength];
	memcpy(this->key, &districtLength, Constants::FIELD_HEADER_SIZE);
	memcpy(this->key + Constants::FIELD_HEADER_SIZE, this->district.c_str(), districtLength);

	return this->key;
}

void DistrictCounts::addCount(char d, char m, short  y, std::string & c, std::string & lName)
{
	CountId ci(d, m, y,c, lName);
	this->counts.push_back(ci);
}

void DistrictCounts::removeCount(char d, char m, short  y, std::string & c, std::string & lName)
{
	vector<CountId> buffer;
	CountId ci;
	while(!this->counts.empty())
	{
		ci = this->counts.at(this->counts.size() - 1);
		this->counts.pop_back();
		if (ci.getDay() == d && ci.getMonth() == m && ci.getYear() == y && ci.getCharge() == c && ci.getListName() == lName)
		{
			break;
		}
		buffer.push_back(ci);
	}

	while(!buffer.empty())
	{
		ci = buffer.at(buffer.size() - 1);
		buffer.pop_back();
		this->counts.push_back(ci);
	}
}

char *DistrictCounts::getBytes()
{
	int countCount = this->counts.size();
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

	memcpy(this->bytes + position, &countCount, sizeof(int));
	position += sizeof(int);

	for(unsigned int i = 0; i < this->counts.size(); i++)
	{
		CountId ci = this->counts.at(i);

		char d = ci.getDay();
		char m = ci.getMonth();
		short y = ci.getYear();
		memcpy(this->bytes + position, &d, sizeof(char));
		position += sizeof(char);
		memcpy(this->bytes + position, &m, sizeof(char));
		position += sizeof(char);
		memcpy(this->bytes + position, &y, sizeof(short));
		position += sizeof(short);
		char chargeSize = ci.getCharge().length() + 1;
		memcpy(this->bytes + position, &chargeSize, Constants::FIELD_HEADER_SIZE);
		position += Constants::FIELD_HEADER_SIZE;
		const char* charge = ci.getCharge().c_str();
		memcpy(this->bytes + position, &charge, chargeSize);
		position += chargeSize;

		char listNameSize = ci.getListName().length() + 1;
		memcpy(this->bytes + position, &listNameSize, Constants::FIELD_HEADER_SIZE);
		position += Constants::FIELD_HEADER_SIZE;
		const char* listName = ci.getListName().c_str();
		memcpy(this->bytes + position, &listName, listNameSize);
	}

	return this->bytes;
}

int DistrictCounts::getKeySize()
{
	char districtLength = this->district.length() + 1;
	return districtLength + Constants::FIELD_HEADER_SIZE;
}

void DistrictCounts::setBytes(const char *value)
{
	this->counts.clear();
	int position = 0;
	char districtLength = 0;

	memcpy(&districtLength, value, Constants::FIELD_HEADER_SIZE);
	position += Constants::FIELD_HEADER_SIZE;
	char dis[districtLength];
	memcpy(dis, value + position, districtLength);
	this->district = dis;
	position += districtLength;

	int countsCount = 0;
	memcpy(&countsCount, value + position, sizeof(int));
	position += sizeof(int);

	for(int i = 0; i < countsCount; i++)
	{
		char d = 0;
		char m = 0;
		short y = 0;
		char chargeSize = 0;
		char listNameSize = 0;

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
		position += chargeSize;

		memcpy(&listNameSize, value + position, Constants::FIELD_HEADER_SIZE);
		position += Constants::FIELD_HEADER_SIZE;

		char listNameBuffer[listNameSize];
		memcpy(listNameBuffer, value + position, listNameSize);

		string charge = chargeBuffer;
		string listName = listNameBuffer;

		CountId ci(d, m, y, charge, listName);
		this->counts.push_back(ci);
	}
}

std::string & DistrictCounts::getDistrict()
{
	return this->district;
}

bool DistrictCounts::hasCounts()
{
	return !this->counts.empty();
}

DistrictCounts::DistrictCounts(const DistrictCounts & other)
{
	this->bytes = NULL;
	this->key = NULL;
	this->counts = other.counts;
	this->district = other.district;
}

DistrictCounts & DistrictCounts::operator=(const DistrictCounts & other)
{
	if (this == &other)
	{
		return *this;
	}

	this->district = other.district;
	this->counts = other.counts;

	if (this->bytes != NULL)
	{
		delete[] this->bytes;
		this->bytes = NULL;
	}

	if (this->key != NULL)
	{
		delete[] this->key;
		this->key = NULL;
	}

	return *this;
}

DistrictCounts::~DistrictCounts()
{
	if (this->bytes != NULL)
	{
		delete[] this->bytes;
	}

	if (this->key != NULL)
	{
		delete[] this->key;
	}
}
