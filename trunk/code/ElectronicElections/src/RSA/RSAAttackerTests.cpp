/*
 * RSAAttackerTests.cpp
 *
 *  Created on: 08/11/2011
 *      Author: lejosdelcielo
 */

#include "RSAAttackerTests.h"
#include "RSAAttacker.h"

RSAAttackerTests::RSAAttackerTests()
{
}

void RSAAttackerTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n"));
}

void RSAAttackerTests::run()
{
	this->printResult("testAttack:", testAttack());
}

bool RSAAttackerTests::testAttack()
{
	RSAAttacker rsaAttacker;
	RSAKey publicKey = {6939446675467, 6939440249215};
	RSAKey privateKey = rsaAttacker.attack(publicKey);
	return true;
}

RSAAttackerTests::~RSAAttackerTests()
{
}

