/*
 * RSAKeySetTests.cpp
 *
 *  Created on: 06/11/2011
 *      Author: lejosdelcielo
 */

#include "RSAKeySetTests.h"
#include <iostream>

using namespace std;


RSAKeySetTests::RSAKeySetTests()
{
}

void RSAKeySetTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n"));
}

void RSAKeySetTests::run()
{
	this->printResult("Key Generation", testGeneration());
}

bool RSAKeySetTests::testGeneration() {
	RSAKeySet rsaks;
	RSAKey privateKey = rsaks.getPrivateKey();
	RSAKey publicKey = rsaks.getPublicKey();
	cout << "phi " << rsaks.phi << endl;
	cout << "privateexp " << privateKey.exp << endl;
	cout << "publicexp " << publicKey.exp << endl;
	cout << ((privateKey.exp%rsaks.phi)*(publicKey.exp%rsaks.phi))%rsaks.phi << endl;
	return (( ((int64)privateKey.exp) * ((int64)publicKey.exp) )%rsaks.phi)==1;
}

RSAKeySetTests::~RSAKeySetTests()
{
}

