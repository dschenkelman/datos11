/*
 * CandidateTests.cpp
 *
 *  Created on: 04/10/2011
 *      Author: lejosdelcielo
 */

#include "CandidateTests.h"
#include "../Entities/Candidate.h"
#include "../VariableBlocks/Constants.h"
#include <cstring>

CandidateTests::CandidateTests()
{
}

void CandidateTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n"));
}

void CandidateTests::run()
{
	printResult("testGetSize", testGetSize());
	printResult("testSetBytes", testSetBytes());
	printResult("testGetBytes", testGetBytes());
}

bool CandidateTests::testGetSize()
{
	std::string name = "Roman";
	std::string charge = "Intendente";
	Candidate c(18, 04, 2009, name, charge, 35094006);

	int size = sizeof(char) * 2 + sizeof(short) + sizeof(int)
			+ Constants::FIELD_HEADER_SIZE + name.size() + 1
			+ Constants::FIELD_HEADER_SIZE + charge.size() + 1;

	return size == c.getSize();
}

bool CandidateTests::testSetBytes()
{
	std::string name = "Lista19";
	std::string charge = "Intendente";

	char bytes[29];

	int i = 0;

	short year = 2009;
	memcpy(bytes+i, &year, sizeof(short)); i += sizeof(short);
	char month = 04;
	memcpy(bytes+i, &month, sizeof(char)); i += sizeof(char);
	char day = 18;
	memcpy(bytes+i, &day, sizeof(char)); i += sizeof(char);

	char chargeLen = charge.size() + 1;
	memcpy(bytes+i, &chargeLen, Constants::FIELD_HEADER_SIZE); i+= Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, charge.c_str(), chargeLen); i+= chargeLen;

	char nameLen = name.size() + 1;
	memcpy(bytes+i, &nameLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, name.c_str(), nameLen); i += nameLen;

	int dni = 35094006;
	memcpy(bytes+i, &dni, sizeof(int));

	Candidate c(22, 06, 1986, "Lista20", "Gobernador", 12121203);
	c.setBytes(bytes);

	if(c.getDay() != day)
	{
		return false;
	}

	if(c.getMonth() != month)
	{
		return false;
	}

	if(c.getYear() != year)
	{
		return false;
	}

	if(c.getName() != name)
	{
		return false;
	}

	if(c.getCharge() != charge)
	{
		return false;
	}

	if(c.getDni() != dni)
	{
		return false;
	}

	return true;
}

bool CandidateTests::testGetBytes()
{
	std::string name = "Lista19"; // 8
	std::string charge = "Intendente"; // 11
	int dni = 35094006;
	short year = 2009;
	char month = 04;
	char day = 18;
	Candidate c(day, month, year, name, charge, dni);

	char bytes[29];

	int i = 0;

	memcpy(bytes+i, &year, sizeof(short)); i += sizeof(short);
	memcpy(bytes+i, &month, sizeof(char)); i += sizeof(char);
	memcpy(bytes+i, &day, sizeof(char)); i += sizeof(char);

	char chargeLen = charge.size() + 1;
	memcpy(bytes+i, &chargeLen, Constants::FIELD_HEADER_SIZE); i+= Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, charge.c_str(), chargeLen); i+= chargeLen;

	char nameLen = name.size() + 1;
	memcpy(bytes+i, &nameLen, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, name.c_str(), nameLen); i += nameLen;

	memcpy(bytes+1, &dni, sizeof(int));

/*	if(strcmp(bytes, c.getBytes()) != 0)
	{
		return false;
	}*/

	Candidate cTwo(11, 11, 2011, "Lista9", "Gobernador", 12121212);
	cTwo.setBytes(c.getBytes());

	if(c.getName() != cTwo.getName())
	{
		return false;
	}

	if(c.getCharge() != cTwo.getCharge())
	{
		return false;
	}

	if(c.getYear() != cTwo.getYear())
	{
		return false;
	}

	if(c.getMonth() != cTwo.getMonth())
	{
		return false;
	}

	if(c.getDay() != cTwo.getDay())
	{
		return false;
	}

	if(c.getDni() != cTwo.getDni())
	{
		return false;
	}

	return true;
}

CandidateTests::~CandidateTests()
{
}
