/*
 * RSACipherTests.cpp
 *
 *  Created on: 06/11/2011
 *      Author: lejosdelcielo
 */

#include "RSACipherTests.h"
#include "RSACipher.h"
#include <iostream>

using namespace std;


RSACipherTests::RSACipherTests()
{
}

void RSACipherTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n"));
}

void RSACipherTests::run()
{
	this->printResult("testGenerateRelativelyPrimeNumbers", testGenerateRelativelyPrimeNumbers());
	//this->printResult("Producto inverso",testProductInverse());
}


bool RSACipherTests::testGenerateRelativelyPrimeNumbers()
{
	RSACipher rsaCipher;
	int64 number = 274877906944;
	int64 res = rsaCipher.getRelativelyPrimeNumber(number);

	if(rsaCipher.GCD(number, res) != 1)
	{
		return false;
	}

	return true;
}

bool RSACipherTests::testProductInverse()
{
	int64 n = 1759;
	int64 d = 550;
	int64 res = RSACipher::productInverse(n,d);
//	cout << "n"<<n<<"d"<<d<<"e"<<res;
	return (res==355);
}

RSACipherTests::~RSACipherTests()
{
}

