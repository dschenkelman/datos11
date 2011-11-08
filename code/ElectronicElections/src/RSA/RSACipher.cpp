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

int64 RSACipher::modularExponentiation(int base, int64 exp, int64 mod) {
	int64 res = 1;
	int64 accumulativeBase = base;
	int64 auxBase;
	int64 bitsCount = 0;//active bits count
	int64 activeBits[64];
	while (exp > 0)
	{
		exp = exp >> 1;
		auxBase = (accumulativeBase*accumulativeBase);
		accumulativeBase = auxBase % mod;
		if(exp % 2 == 1)
		{
			activeBits[bitsCount] = accumulativeBase;
			bitsCount++;
		}
	}
	if(bitsCount == 0) return 0; //it shouldn't return

	res = activeBits[0];
	for(int i= 1; i< bitsCount; i++)
	{
		auxBase = activeBits[i] * res;
		res = auxBase % mod;
	}
	return res;
}

void RSACipher::cipherMessage(char* message, int64 expKey, int64 n, char* cipheredMessage)
{
	int64 greaterKey = n;
	int64 greaterBit = 1;
	int64 messageLen = strlen(message)+1;

	while(greaterKey != 1)
	{
		greaterBit++;
		greaterKey = greaterKey >> 1;
	}
	//check greaterBit to be multiple of 8 in order to not cut bytes
	if(greaterBit % 8 != 0)
	{
		int offset = (greaterBit % 8);
		greaterBit-= offset;
	}
	//tengo el valor de greater bit, divido el message
	int blocksMessage = messageLen *8 /greaterBit +1; //tengo la cantidad de divisiones del mensaje en BYTES!!
	int64 crypt;
	int64 blockLen = greaterBit/8; //len in BYTES!
	int block;

	char cryptMessage[messageLen];
	int messageOffset = (messageLen % blockLen);
	memcpy(&block, message, messageOffset);
	crypt = modularExponentiation(block, expKey, n);
	memcpy(cryptMessage, &crypt, messageOffset);
	for(int i=0;i< blocksMessage-1; i++)
	{
		memcpy(&block, message+ messageOffset+(i*blockLen), blockLen);
		crypt = modularExponentiation(block, expKey, n);
		memcpy(cryptMessage+ messageOffset+(i*blockLen), &crypt, blockLen);
	}

	strcpy(cipheredMessage, cryptMessage);
	return;
}


RSACipher::~RSACipher() {
}

