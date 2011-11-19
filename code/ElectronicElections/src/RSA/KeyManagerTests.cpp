/*
 * KeyManagerTests.cpp
 *
 *  Created on: 17/11/2011
 *      Author: lejosdelcielo
 */

#include "KeyManagerTests.h"
#include <iostream>
#include <string>

KeyManagerTests::KeyManagerTests()
{
}

void KeyManagerTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n"));
}

void KeyManagerTests::run()
{
	this->printResult("testGenerate", this->testGenerate());
}

bool KeyManagerTests::testGenerate()
{
	KeyManager keyManager(8);
	keyManager.generate();

	RSAKey publicKey = keyManager.getPublicKey();
	RSAKey privateKey = keyManager.getPrivateKey();

	KeyManager keyManagerTwo(8);

	bool conditionOne = (publicKey.n == keyManagerTwo.getPublicKey().n);
	bool conditionTwo = (publicKey.exp == keyManagerTwo.getPublicKey().exp);
	bool conditionThree = (privateKey.n == keyManagerTwo.getPrivateKey().n);
	bool conditionFour = (privateKey.exp == keyManagerTwo.getPrivateKey().exp);

	return (conditionOne && conditionTwo && conditionThree && conditionFour);
}

KeyManagerTests::~KeyManagerTests()
{
}

