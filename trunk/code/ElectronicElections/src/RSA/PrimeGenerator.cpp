/*
 * PrimeGenerator.cpp
 *
 *  Created on: 06/11/2011
 *      Author: alejandro
 */

#include "PrimeGenerator.h"
#include <cstdlib>
#include <time.h>
#include "../Helpers/ByteOperators.h"
#include <cstring>
#include <iostream>
#include <math.h>

using namespace std;

PrimeGenerator::PrimeGenerator(int end)
{
	srand( time(NULL) );
	this->end = end;
	int byteCount = end/8.0;
	unsigned char* primes = new unsigned char[byteCount];
	// initial setup: all true
	memset(primes, 255, byteCount);

	this->total = end-1;
	// starting algorithm
	for (int i=2;i<=end/2;i++)
	{
		unsigned int byteNumberI = i / 8;
		char bitNumber = (i - 1) % 8;

		if (ByteOperators::isBitOne(primes[byteNumberI], bitNumber) == true)
		{
			for (int j=2*i; (j < end);j+=i)
			{
				if (j <= 0)
				{
					break;
				}
				unsigned int byteNumberJ = j / 8;
				bitNumber = (j - 1) % 8;
				if (ByteOperators::isBitOne(primes[byteNumberJ], bitNumber)) this->total--;
				ByteOperators::setBit((char*)primes + byteNumberJ, bitNumber, 0);
			}
		}
	}

	for (int i=2;i < this->end;i++)
	{
		unsigned int byteNumberI = i / 8;
		char bitNumber = (i - 1) % 8;
		if (ByteOperators::isBitOne(primes[byteNumberI], bitNumber))
		{
			this->primeNumbers.push_back(i);
		}
	}

	delete primes;
}

int PrimeGenerator::getRandomInRange(int64 minimum, int64 maximum, int64* p, int64* q)
{
	int max = this->primeNumbers[this->primeNumbers.size() - 1];
	if (max < minimum) return -1;
	int num;
	do
	{
		num = rand()%total;
		*p = this->primeNumbers[num];
	} while (*p < minimum || *p > maximum);

	do
	{
		num = rand()%total;
		*q = this->primeNumbers[num];
	} while (*q < minimum || *q == *p || *q > maximum);
}

PrimeGenerator::~PrimeGenerator()
{
}
