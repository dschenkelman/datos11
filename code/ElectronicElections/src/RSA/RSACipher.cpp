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

vector<int64> RSACipher::generateRelativelyPrimeNumbers(int64 number)
{
	//if isPrime(number) return number-1;
	vector<int64> coprimeNumbers;
	int64 jump = (int64) rand();

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
		jump = (int64)rand();
	}

	return coprimeNumbers;
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

void RSACipher::cipherMessage(char* message, int64 expKey, int64 n, char* cipheredMessage, int64 messageLen)
{
	int64 block64=0;
	int64 crypt=0;
	//char cryptMessage[messageLen];
	//memset(cryptMessage, 0, messageLen);
	char realMessage[8];
	int offset = 8- messageLen;
	memset(realMessage, 0, 8);
	memcpy(realMessage, message, messageLen);
	memset(cipheredMessage, 0, messageLen);
	block64= 0;
	crypt=0;
	memcpy(&block64, realMessage, messageLen);
	crypt = modularExponentiation(block64, expKey, n);
	memcpy(cipheredMessage, &crypt, 8);
	return;


	/*for(int i=0;i< blocksMessage; i++)
	{
		block64= 0;
		crypt=0;
		memcpy(&block64, message+(i*blockLen), blockLen);
		//memcpy(&block64, message+ messageOffset+(i*blockLen), blockLen);
		crypt = modularExponentiation(block64, expKey, n);
		memcpy(cryptMessage+(i*blockLen), &crypt, blockLen);
		//0memcpy(cryptMessage+ messageOffset+(i*blockLen), &crypt, blockLen);
	}
	//cryptMessage[messageLen+1] = '\0';
	memcpy(cipheredMessage, cryptMessage, blockLen);*/
}


RSACipher::~RSACipher() {
}

