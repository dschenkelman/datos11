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
	RSAKeySet rsaKey(8);
	RSAKey publicKey= rsaKey.getPublicKey();
	RSAKey privateKey= rsaKey.getPrivateKey();

	int64 criptKey = publicKey.exp;
	int64 decriptKey = privateKey.exp;
	int64 n = publicKey.n;
	int len = 5;
	char passwords[10][5] = {"asdf", "qwer", "abcd", "efgh", "dami", "gaby", "juan", "alet", "pass", "pas2"};
	int chunkSize = rsaCipher.getChunkSize(n) + 1;
	int chunks = ceil(len / (float)(chunkSize - 1));

	char crypts[10][chunks * chunkSize];
	char charMessage[len];
	char cipheredMessage[100];
	char decriptedMessage[100];

	for(int i=0;i<10;i++){
		memcpy(charMessage, passwords[i], len);
		rsaCipher.cipherMessage(charMessage, criptKey, n, cipheredMessage, len);
		memset(crypts[i], 0, chunkSize);
		memcpy(crypts[i], cipheredMessage, chunks * chunkSize);
	}
	char newMessage[len];
	for(int i=0;i<10;i++){
		rsaCipher.decryptMessage(crypts[i], decriptKey, n, decriptedMessage, chunks * chunkSize);
		memset(newMessage, 0, len);
		memcpy(newMessage, decriptedMessage, len);
		if (strcmp(passwords[i], newMessage) !=0 )
		{
			return false;
		}

	}
	return true;
}

RSACipherTests::~RSACipherTests()
{
}

