/*
 * RSACipher.cpp
 *
 *  Created on: 06/11/2011
 *      Author: lejosdelcielo
 */

#include "RSACipher.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string.h>

using namespace std;

RSACipher::RSACipher()
{
	srand(time(NULL));
}

int64 RSACipher::GCD(int64 numberOne, int64 numberTwo)
{
	int64 mod = numberOne % numberTwo;

	if (mod == 0)
	{
		return numberTwo;
	}

	return GCD(numberTwo, mod);
}

vector<int64> RSACipher::generateRelativelyPrimeNumbers(int64 number)
{
	//if isPrime(number) return number-1;
	vector<int64> coprimeNumbers;

	for (int64 i = (number - 2); i > 0; i--)
	{
		if (this->GCD(number, i) == 1)
		{
			if(coprimeNumbers.size() < 500000 || i == 1)
			{
				coprimeNumbers.push_back(i);
			}

			else
			{
				return coprimeNumbers;
			}
		}
	}
}

int64 RSACipher::getRelativelyPrimeNumber(int64 number)
{
	vector<int64> coprimeNumbers = this->generateRelativelyPrimeNumbers(number);
	int randomNumber = rand() % coprimeNumbers.size();

	return coprimeNumbers.at(randomNumber);
}

int64 RSACipher::productInverse(int64 phi, int64 d) { // mod, base
	int64 olda1 = 1;
	int64 olda2 = 0;
	int64 olda3 = phi;
	int64 oldb1 = 0;
	int64 oldb2 = 1;
	int64 oldb3 = d;
	int64 a1,a2,a3,b1,b2,b3;
	int64 q;
	do {
		q = olda3/oldb3;
		a1 = oldb1;
		a2 = oldb2;
		a3 = oldb3;
		b1 = olda1 - q*oldb1;
		b2 = olda2 - q*oldb2;
		b3 = olda3 - q*oldb3;
		olda1 = a1;
		olda2 = a2;
		olda3 = a3;
		oldb1 = b1;
		oldb2 = b2;
		oldb3 = b3;
//		cout << " a1 " << a1 << " a2 " << a2 << " a3 " << a3 << " b1 " << b1 << " b2 " << b2 << " b3 " << b3 << endl;
	} while((b3!=1)&&(b3!=0));

	if (b3==1)
	{
		if(b2 < 0)
		{
			b2 += phi;
		}

		return b2;
	}

	return -1;
}

#include <math.h>

int64 RSACipher::modularExponentiation(int64 base, int64 exp, int64 mod) {
	int64 res = 1;
	while (exp>0) {
		if ((exp&1) == 1) res = (res*base)%mod;
		exp = exp >> 1;
		base = (base*base)%mod;
	}
	return res;
}

void RSACipher::cipherMessage(char* message, int64 expKey, int64 n, char* cipheredMessage)
{
	int64 greaterKey = n;
	int64 greaterBit = 1;
	int64 messageLen = strlen(message);

	while(greaterKey != 1)
	{
		greaterBit++;
		greaterKey = greaterKey >> 1;
	}

	//tengo el valor de greater bit, divido el message
	int blocksMessage = messageLen *8 /greaterBit +1; //tengo la cantidad de divisiones del mensaje en BYTES!!
	int64 crypt;
	int64 blockLen = greaterBit/8 +1;
	int64 block;
	char cryptMessage[messageLen];
	for(int i=0;i< blocksMessage; i++)
	{
		memcpy(&block, message+ (i*blockLen), blockLen);
		crypt = modularExponentiation(block, expKey, n);
		memcpy(cryptMessage+ (i*blockLen), &crypt, blockLen);
	}

	strcpy(cipheredMessage, cryptMessage);
	return;
}


RSACipher::~RSACipher() {
}

