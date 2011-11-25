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
#include <math.h>
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

vector<int64> RSACipher::generateRelativelyPrimeNumbers(int64 number, int keySize)
{
	//if isPrime(number) return number-1;
	vector<int64> coprimeNumbers;
	int64 jump = 0;
	if (keySize >= 6)
	{
		jump = (int64) rand();
	}
	else if (keySize >= 4 && keySize < 6)
	{
		jump = (int64) rand() % 65536;
	}
	else
	{
		jump = 1;
	}


	for (int64 i = (number - 2); i > 1; i-= jump)
	{
		if (this->GCD(number, i) == 1)
		{
			if(coprimeNumbers.size() < 50000)
			{
				coprimeNumbers.push_back(i);
			}
			else
			{
				return coprimeNumbers;
			}
		}

		if (keySize >= 6)
		{
			jump = (int64) rand();
		}
		else if (keySize >= 4 && keySize < 6)
		{
			jump = (int64) rand() % 65536;
		}
		else
		{
			jump = 1;
		}
	}

	return coprimeNumbers;
}

int64 RSACipher::getRelativelyPrimeNumber(int64 number, int keySize)
{
	vector<int64> coprimeNumbers = this->generateRelativelyPrimeNumbers(number, keySize);
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

int64 mul64modulingMod(int64 a, int64 b, int64 c)
{
	int64 x = 0,y=a%c;
	while(b > 0){
		if(b%2 == 1){
			x = (x+y)%c;
		}
		y = (y*2)%c;
		b /= 2;
	}
	return x%c;
}

int64 RSACipher::modularExponentiation(int64 base, int64 exp, int64 mod) {
	int64 res = 1;
	int64 accumulativeBase = base;
	while (exp > 0)
	{
		if((exp & 1) == 1)
		{
			res = mul64modulingMod(accumulativeBase, res, mod);
		}
		exp = exp >> 1;
		accumulativeBase = mul64modulingMod(accumulativeBase, accumulativeBase, mod);
	}
	return res;
}

int RSACipher::getChunkSize(int64 n)
{
	int chunkSize = 0;
	int64 shifter = 1;
	for (unsigned int i = 0; i < sizeof(int64) * 8; i++)
	{
		if ((n & (shifter << i)) != 0)
		{
			chunkSize = i / 8;
		}
	}

	return chunkSize;
}

void RSACipher::cipherMessage(char* message, int64 expKey, int64 n, char* cipheredMessage, int64 messageLen)
{
	int chunkSize = this->getChunkSize(n);
	int i = 0;
	int position = 0;
	int chunks = ceil(messageLen / (float)chunkSize);
	while (i < chunks)
	{
		char chunk[chunkSize];
		memset(chunk, 0, chunkSize);

		if (chunkSize + position > messageLen)
		{
			memcpy(chunk, message + position, messageLen-position);
		}
		else
		{
			memcpy(chunk, message + position, chunkSize);
		}

		int64 messageNumber = 0;
		memcpy(&messageNumber, chunk, chunkSize);
		int64 crypt = modularExponentiation(messageNumber, expKey, n);
		memcpy(cipheredMessage + position + i, &crypt, chunkSize + 1);

		position += chunkSize;
		i++;
	}
	return;
}

void RSACipher::decryptMessage(char* cipheredMessage, int64 expKey, int64 n, char* decryptedMessage, int64 messageLen)
{
	int chunkSize = this->getChunkSize(n);
	chunkSize += 1;
	int i = 0;
	int position = 0;
	while (position < messageLen)
	{
		char chunk[chunkSize];
		memset(chunk, 0, chunkSize);

		if (chunkSize + position > messageLen)
		{
			memcpy(chunk, cipheredMessage + position, messageLen-position);
		}
		else
		{
			memcpy(chunk, cipheredMessage + position, chunkSize);
		}

		int64 messageNumber = 0;
		memcpy(&messageNumber, chunk, chunkSize);
		int64 crypt = modularExponentiation(messageNumber, expKey, n);
		memcpy(decryptedMessage + position - i, &crypt, chunkSize);

		position += chunkSize;
		i++;
	}
}

RSACipher::~RSACipher() {
}
