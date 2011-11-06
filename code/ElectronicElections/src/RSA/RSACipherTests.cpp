/*
 * RSACipherTests.cpp
 *
 *  Created on: 06/11/2011
 *      Author: lejosdelcielo
 */

#include "RSACipherTests.h"
#include "RSAKeySet.h"
#include <iostream>
#include <string.h>

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
	//this->printResult("testGenerateRelativelyPrimeNumbers", testGenerateRelativelyPrimeNumbers());
	//this->printResult("Producto inverso",testProductInverse());
	//this->printResult("Multiplicacion exponencial",testModularExponentiation());
	this->printResult("Encriptacion",testCipherMessage());
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

bool RSACipherTests::testModularExponentiation() {
	int base = 77;
	int exp = 103;
	int mod = 138;
	int res = RSACipher::modularExponentiation(base,exp,mod);
	return (res==71);
}

bool RSACipherTests::testCipherMessage()
{
	string message ="abcd";
	RSACipher rsaCipher;
	RSAKeySet rsaKey;
	RSAKey publicKey= rsaKey.getPublicKey();
	RSAKey privateKey= rsaKey.getPrivateKey();

	int64 criptKey = publicKey.exp;
	int64 decriptKey = privateKey.exp;
	int64 n = publicKey.n;
	char* charMessage = (char*)message.c_str();
	char cipheredMessage[message.length()];
	char decriptedMessage[message.length()];
	rsaCipher.cipherMessage(charMessage, criptKey, n, cipheredMessage);
	rsaCipher.cipherMessage(cipheredMessage, decriptKey, n, decriptedMessage);

	if (strcmp(charMessage, decriptedMessage) ==0 )
	{
		return true;
	}
	return false;
}

RSACipherTests::~RSACipherTests()
{
}

