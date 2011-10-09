/*
 * VoterTests.cpp
 *
 *  Created on: 07/10/2011
 *      Author: lejosdelcielo
 */

#include "VoterTests.h"
#include "../Entities/Voter.h"
#include "../VariableBlocks/Constants.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

VoterTests::VoterTests() {
	// TODO Auto-generated constructor stub

}

void VoterTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n"));
}

void VoterTests::run()
{
	printResult("testGetSize", testGetSize());
	printResult("testGetKeySize", testGetKeySize());
	printResult("testGetKey", testGetKey());
	printResult("testSetBytes",testSetBytes());
	printResult("testGetBytes", testGetBytes());
}

bool VoterTests::testGetSize()
{
	ElectionKey electOne = {2000, 12, 12, "Intendente"};
	ElectionKey electTwo = {1990, 11, 11, "Gobernador"};
	int dni = 35094006;
	std::string names = "Juan Roman Riquelme";
	std::string pass = "diez";
	std::string address = "Santa Fe 1999";
	std::string district = "Buenos Aires";

	std::vector<ElectionKey> list;
	list.push_back(electOne); list.push_back(electTwo);

	Voter v(dni, names, pass, address, district, list);

	int size = sizeof(dni);
	size += names.size() + 1 + Constants::FIELD_HEADER_SIZE;
	size += pass.size() + 1;
	size += address.size() + 1 + Constants::FIELD_HEADER_SIZE;
	size += district.size() + 1 + Constants::FIELD_HEADER_SIZE;
	size += Constants::FIELD_HEADER_SIZE;
	size += sizeof(electOne.day) + sizeof(electOne.month) + sizeof(electOne.year) + 1 + Constants::FIELD_HEADER_SIZE + electOne.charge.size();
	size += sizeof(electTwo.day) + sizeof(electTwo.month) + sizeof(electTwo.year) + 1 + Constants::FIELD_HEADER_SIZE + electTwo.charge.size();

	if(size != v.getSize())
	{
		return false;
	}

	return true;
}

bool VoterTests::testGetKeySize()
{
	ElectionKey electOne = {2000, 12, 12, "Intendente"};
	ElectionKey electTwo = {1990, 11, 11, "Gobernador"};
	int dni = 35094006;
	std::string names = "Juan Roman Riquelme";
	std::string pass = "diez";
	std::string address = "Santa Fe 1999";
	std::string district = "Buenos Aires";

	std::vector<ElectionKey> list;
	list.push_back(electOne); list.push_back(electTwo);

	Voter v(dni, names, pass, address, district, list);

	if(v.getKeySize() != sizeof(dni))
	{
		return false;
	}

	return true;
}

bool VoterTests::testGetKey()
{
	ElectionKey electOne = {2000, 12, 12, "Intendente"};
	ElectionKey electTwo = {1990, 11, 11, "Gobernador"};
	int dni = 35094006;
	std::string names = "Juan Roman Riquelme";
	std::string pass = "diez";
	std::string address = "Santa Fe 1999";
	std::string district = "Buenos Aires";

	std::vector<ElectionKey> list;
	list.push_back(electOne); list.push_back(electTwo);

	Voter v(dni, names, pass, address, district, list);

	char k[4];
	memcpy(k, &dni, sizeof(int));

	if(strcmp(v.getKey(), k) != 0)
	{
		return false;
	}

	return true;
}

bool VoterTests::testSetBytes()
{
	std::vector<ElectionKey> list;
	Voter v(11111111, "Fernando Gago", "1234", "Cordoba 900", "Neuquen", list);

	int dni = 35094006;
	std::string names = "Juan Roman Riquelme";
	std::string pass = "diez";
	std::string address = "Santa Fe 1999";
	std::string district = "Buenos Aires";

	ElectionKey ekOne = {2000, 12, 12, "Intendente"};
	ElectionKey ekTwo = {2003, 11, 10, "Presidente"};

	int size = sizeof(dni);
	size += Constants::FIELD_HEADER_SIZE + 1 + names.size();
	size += 1 + pass.size();
	size += Constants::FIELD_HEADER_SIZE + 1 + address.size();
	size += Constants::FIELD_HEADER_SIZE + 1 + district.size();
	size += Constants::FIELD_HEADER_SIZE;
	size += sizeof(short) + sizeof(char) * 2 + Constants::FIELD_HEADER_SIZE + 1 + ekOne.charge.size();
	size += sizeof(short) + sizeof(char) * 2 + Constants::FIELD_HEADER_SIZE + 1 + ekTwo.charge.size();

	char bytes[size];

	int i = 0;

	memcpy(bytes+i, &dni, sizeof(int)); i += sizeof(int);

	char len = names.size() + 1;
	memcpy(bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, names.c_str(), len); i += len;

	len = 4; // pass len
	memcpy(bytes+i, pass.c_str(), len); i += len;

	len = address.size() + 1;
	memcpy(bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, address.c_str(), len); i += len;

	len = district.size() + 1;
	memcpy(bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, district.c_str(), len); i += len;

	len = 2; //list size
	memcpy(bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;

	// first element
	memcpy(bytes+i, &(ekOne.year), sizeof(short)); i += sizeof(short);
	memcpy(bytes+i, &(ekOne.month), sizeof(char)); i += sizeof(char);
	memcpy(bytes+i, &(ekOne.day), sizeof(char)); i += sizeof(char);
	len = ekOne.charge.size() + 1;
	memcpy(bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, ekOne.charge.c_str(), len); i += len;

	// second element
	memcpy(bytes+i, &(ekTwo.year), sizeof(short)); i += sizeof(short);
	memcpy(bytes+i, &(ekTwo.month), sizeof(char)); i += sizeof(char);
	memcpy(bytes+i, &(ekTwo.day), sizeof(char)); i += sizeof(char);
	len = ekTwo.charge.size() + 1;
	memcpy(bytes+i, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, ekTwo.charge.c_str(), len); i += len;

	v.setBytes(bytes);

	if(v.getAddress() != address)
	{
		return false;
	}

	if(v.getDistrict() != district)
	{
		return false;
	}

	if(v.getDni() != dni)
	{
		return false;
	}

	if(v.getNames() != names)
	{
		return false;
	}

	if(v.getPassword() != pass)
	{
		return false;
	}

	if(v.getElectionKeyList().at(0).charge != ekOne.charge)
	{
		return false;
	}

	if(v.getElectionKeyList().at(0).year != ekOne.year)
	{
		return false;
	}

	if(v.getElectionKeyList().at(0).month != ekOne.month)
	{
		return false;
	}

	if(v.getElectionKeyList().at(0).day != ekOne.day)
	{
		return false;
	}

	if(v.getElectionKeyList().at(1).charge != ekTwo.charge)
	{
		return false;
	}

	if(v.getElectionKeyList().at(1).year != ekTwo.year)
	{
		return false;
	}

	if(v.getElectionKeyList().at(1).month != ekTwo.month)
	{
		return false;
	}

	if(v.getElectionKeyList().at(1).day != ekTwo.day)
	{
		return false;
	}

	return true;
}

bool VoterTests::testGetBytes()
{
	ElectionKey electOne = {2000, 12, 12, "Intendente"};
	ElectionKey electTwo = {1990, 11, 11, "Gobernador"};
	int dni = 35094006;
	std::string names = "Juan Roman Riquelme";
	std::string pass = "diez";
	std::string address = "Santa Fe 1999";
	std::string district = "Buenos Aires";

	std::vector<ElectionKey> list;
	list.push_back(electOne); list.push_back(electTwo);

	Voter vOne(dni, names, pass, address, district, list);

	std::vector<ElectionKey> listTwo;
	Voter vTwo(11111111, "Fernando Gago", "1234", "Cordoba 900", "Neuquen", listTwo);

	vTwo.setBytes(vOne.getBytes());

	if(vOne.getAddress() != vTwo.getAddress())
	{
		return false;
	}

	if(vOne.getDistrict() != vTwo.getDistrict())
	{
		return false;
	}

	if(vOne.getDni() != vTwo.getDni())
	{
		return false;
	}

	if(vOne.getNames() != vTwo.getNames())
	{
		return false;
	}

	if(vOne.getPassword() != vTwo.getPassword())
	{
		return false;
	}

	if(vOne.getElectionKeyList().size() != vTwo.getElectionKeyList().size())
	{
		return false;
	}

	for(int j = 0; j < vOne.getElectionKeyList().size(); j++)
	{
		if(vOne.getElectionKeyList().at(j).charge != vTwo.getElectionKeyList().at(j).charge)
		{
			return false;
		}

		if(vOne.getElectionKeyList().at(j).year != vTwo.getElectionKeyList().at(j).year)
		{
			return false;
		}

		if(vOne.getElectionKeyList().at(j).month != vTwo.getElectionKeyList().at(j).month)
		{
			return false;
		}

		if(vOne.getElectionKeyList().at(j).day != vTwo.getElectionKeyList().at(j).day)
		{
			return false;
		}
	}

	return true;
}

VoterTests::~VoterTests() {
	// TODO Auto-generated destructor stub
}

