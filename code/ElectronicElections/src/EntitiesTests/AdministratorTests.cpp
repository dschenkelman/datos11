/*
 * AdministratorTests.cpp
 *
 *  Created on: 04/10/2011
 *      Author: lejosdelcielo
 */

#include "AdministratorTests.h"
#include "../VariableBlocks/Constants.h"
#include "../Entities/Administrator.h"
#include <cstring>
#include <iostream>

using namespace std;

AdministratorTests::AdministratorTests()
{
}

void AdministratorTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n"));
}

void AdministratorTests::run()
{
	printResult("testGetSize", testGetSize());
	printResult("testGetKeySize", testGetKeySize());
	printResult("testSetBytes", testSetBytes());
	printResult("testGetBytes", testGetBytes());
	printResult("testgetKey", testGetKey());
}

bool AdministratorTests::testGetSize()
{
	std::string name = "Roman";
	std::string pass = "123456";
	Administrator adm(name, pass);

	int size = name.size() + Constants::FIELD_HEADER_SIZE + 1 + 5; // 5 por 4 del pass + el /0.

	if(adm.getSize() != size)
	{
		return false;
	}

	return true;
}

bool AdministratorTests::testGetKeySize()
{
	std::string name = "Roman";
	std::string pass = "123456";
	Administrator adm(name, pass);

	int size = name.size() + 1 + Constants::FIELD_HEADER_SIZE;

	return size == adm.getKeySize();
}

bool AdministratorTests::testSetBytes()
{
	std::string name = "Roman";
	std::string pass = "123456";
	Administrator adm("Invalid", "Invalid");

	char bytes[12];
	char len = name.size() + 1 + Constants::FIELD_HEADER_SIZE;
	int i = 0;

	memcpy(bytes, &len, Constants::FIELD_HEADER_SIZE); i += Constants::FIELD_HEADER_SIZE;
	memcpy(bytes+i, name.c_str(), len); i += len;
	memcpy(bytes+i, pass.c_str(), 4);

	adm.setBytes(bytes);

	if(adm.getName() != name)
	{
		return false;
	}

	if(adm.getPassword() != pass.substr(0, 4))
	{
		return false;
	}

	return true;
}

bool AdministratorTests::testGetBytes()
{
	std::string name = "Roman";
	std::string pass = "123456";
	Administrator adm(name, pass);

	Administrator admTwo("Invalid", "Invalid");
	admTwo.setBytes(adm.getBytes());

	if(adm.getName() != admTwo.getName())
	{
		return false;
	}

	if(adm.getPassword() != admTwo.getPassword())
	{
		return false;
	}

	return true;
}

bool AdministratorTests::testGetKey()
{
	std::string name = "Roman";
	std::string pass = "123456";
	Administrator adm(name, pass);

	int size = adm.getKeySize();

	char key[size];
	memcpy(key, &size, Constants::FIELD_HEADER_SIZE);
	memcpy(key+Constants::FIELD_HEADER_SIZE, name.c_str(), size);

	if(strcmp(adm.getKey(), key) != 0)
	{
		return false;
	}

	return true;
}

AdministratorTests::~AdministratorTests()
{
}

