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

int64 mulsame64modulingMod(int64 a, int64 b, int64 c)
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

	/*uInt64 auxBase;

	unsigned int hi1, hi2, low1, low2;
	unsigned int a, b, c, d; //number of 128 bits divided
	unsigned int secondLow, secondHi, thirdLow, thirdHi, fourthLow, fourthHi;
	uInt64 firstMul,secondMul, thirdMul, fourthMul;
	uInt64 b64, c64, d64; //hi piece goes to the next number. d64 doesn't have hi piece.
	uInt64 baseHi, baseLow; // low = a|b; hi = c|d;
	uInt64 aux64L1 = 1;
	uInt64 maxInt64 = 0xffffffffffffffff;

	low1 = (int)(op1);
	low2 = (int)(op2);
	hi1 = (int)(op1 >> 32);
	hi2 =(int)(op2 >> 32);
	//firstMul = 0;
	firstMul = aux64L1* low1 * low2;
	a = (int) firstMul; //first low number of 128 bits
	b64 = (firstMul>>32);
	//secondMul = 0;
	secondMul = aux64L1* low1 * hi2;
	secondLow = (int)secondMul;
	secondHi = (int)(secondMul>> 32);//to add it in c number, and check overflow
	//thirdMul = 0;
	thirdMul = aux64L1* low2 * hi1;
	thirdLow = (int)thirdMul;
	thirdHi = (int)(thirdMul>> 32);//to add it in c number, and check overflow
	//fourthMul = 0;
	fourthMul = aux64L1* hi1 * hi2;
	fourthLow = (int)fourthMul;
	fourthHi = (int)(fourthMul>> 32);//to add it in d number, no overflow

	//add and find b and c
	b64+= secondLow;
	b64+= thirdLow;
	b = (int)b64;
	c64 = (b64>> 32);
	c64+= secondHi;
	c64+= thirdHi;
	c64+= fourthLow;
	c = (int)c64;
	d64 = (c64>> 32);
	//find d
	d64+= fourthHi;
	d = (int)d64;

	//check overflow in 128 bits
	baseLow = b;
	baseLow = baseLow<< 32;
	baseLow = baseLow | a;
	baseHi = d;
	baseHi = baseHi<< 32;
	baseHi = baseHi | c;

	//erase overflow decreasing mod
	while(baseHi > 0)
	{
		if(baseLow < mod)
		{
			baseHi--;
			baseLow+= ((maxInt64) % mod);
		}
		else
		{
			baseLow = baseLow - mod;
		}
	}
	return baseLow % mod;*/
}

int64 RSACipher::modularExponentiation(int64 base, int64 exp, int64 mod) {
	cout << "start modexp" << endl << "base " << base << " exp " << exp << " mod " << mod << endl;
	int64 res = 1;
	//uInt64 bitsCount = 0;//active bits count
	//uInt64 activeBits[64];
	int64 accumulativeBase = base;
	while (exp > 0)
	{
		if((exp & 1) == 1)
		{
			cout << "start mult" << " a " << accumulativeBase << " b " << res << " mod " << mod << endl;
			res = mulsame64modulingMod(accumulativeBase, res, mod);
			cout << "end mult res " << res << endl;
		}
		exp = exp >> 1;
		accumulativeBase = mulsame64modulingMod(accumulativeBase, accumulativeBase, mod);
	}
	cout << "end modexp res " << res << endl;
	return res;
}

void RSACipher::cipherMessage(char* message, int64 expKey, int64 n, char* cipheredMessage, int64 messageLen)
{
	int64 greaterKey = n;
	int64 greaterBit = 1;

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
	int64 crypt=0;
	int64 truncCrypt = 0;
	int64 blockLen = greaterBit/8; //len in BYTES!
	int64 block64=0;

	char cryptMessage[messageLen];
	memset(cryptMessage, 0, messageLen);
	int messageOffset = (messageLen % blockLen);
	char block[messageOffset];
	memcpy(block, message, messageOffset);
	block64 = *( (int64*)block );
	crypt = modularExponentiation(block64, expKey, n);
	//strncpy(cryptMessage, (char*)crypt,messageOffset);
	memcpy(&truncCrypt, &crypt, messageOffset);
	memcpy(cryptMessage, &crypt, messageOffset);
	for(int i=0;i< blocksMessage-1; i++)
	{
		block64= 0;
		crypt=0;
		memset(cryptMessage, 0, messageLen);
		memcpy(&block64, message+ messageOffset+(i*blockLen), blockLen);
		crypt = modularExponentiation(block64, expKey, n);
		memcpy(cryptMessage+ messageOffset+(i*blockLen), &crypt, blockLen);
	}
	//cryptMessage[messageLen+1] = '\0';
	strcpy(cipheredMessage, cryptMessage);
	return;
}


RSACipher::~RSACipher() {
}
