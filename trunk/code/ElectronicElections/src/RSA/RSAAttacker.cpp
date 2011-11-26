/*
 * RSAAttacker.cpp
 *
 *  Created on: 08/11/2011
 *      Author: lejosdelcielo
 */

#include "RSAAttacker.h"
#include "RSACipher.h"
#include "RSAKey.h"
#include <iostream>

using namespace std;

RSAAttacker::RSAAttacker()
{
	this->intMaxValue = numeric_limits<unsigned int>::max();
}

void RSAAttacker::getPrimeNumbers(int64 n, int64* p, int64* q, int keySize)
{
	unsigned int limit = this->intMaxValue >> (32 - keySize * 4);

	for(unsigned int i = limit; i > 0; i--)
	{
		if ((n % i) == 0)
		{
			*p = i;
			*q = n / (*p);
			break;
		}
	}
}

RSAKey RSAAttacker::attack(RSAKey& publicKey, int keySize)
{
	int64 p, q;
	this->getPrimeNumbers(publicKey.n, &p, &q, keySize);
	int64 phi = (q - 1) * (p - 1);

	int64 e = publicKey.exp;
	int64 d = RSACipher::productInverse(phi, e);

	RSAKey privateKey = {publicKey.n, d};

	return privateKey;
}

RSAAttacker::~RSAAttacker()
{
}

