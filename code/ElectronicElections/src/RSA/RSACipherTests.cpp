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
	RSACipher rsaCipher;
	RSAKeySet rsaKey;
	RSAKey publicKey= rsaKey.getPublicKey();
	RSAKey privateKey= rsaKey.getPrivateKey();

	int64 criptKey = publicKey.exp;
	int64 decriptKey = privateKey.exp;
	int64 n = publicKey.n;
	int len = 5;
	char passess[10][5] = {"asdf", "qwer", "abcd", "efgh", "dami", "gaby", "juan", "alet", "pass", "pas2"};
	char crypts[10][8];
	char charMessage[len];
	char cipheredMessage[8];
	char decriptedMessage[8];
	for(int i=0;i<10;i++){
		memcpy(charMessage, passess[i], len);
		rsaCipher.cipherMessage(charMessage, criptKey, n, cipheredMessage, len);
		memset(crypts[i], 0, 8);
		memcpy(crypts[i], cipheredMessage,8);
	}
	char newMessage[len];
	for(int i=0;i<10;i++){
		rsaCipher.cipherMessage(crypts[i], decriptKey, n, decriptedMessage, 8);
		memset(newMessage, 0, len);
		memcpy(newMessage, decriptedMessage, len);
		if (strcmp(passess[i], newMessage) !=0 )
		{
			return false;
		}

	}
	return true;
}

RSACipherTests::~RSACipherTests()
{
}

