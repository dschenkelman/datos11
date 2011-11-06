/*
 * RSACipher.h
 *
 *  Created on: 06/11/2011
 *      Author: lejosdelcielo
 */

#ifndef RSACIPHER_H_
#define RSACIPHER_H_

#include "RSAKey.h"
#include <vector>

using namespace std;

class RSACipher {
private:
	vector<int64> generateRelativelyPrimeNumbers(int64 number);
public:
	RSACipher();
	int64 GCD(int64 numberOne, int64 numberTwo);
	int64 getRelativelyPrimeNumber(int64 number);
	static int64 productInverse(int64 n, int64 d);
	static int64 modularExponentiation(int64 a, int64 b, int64 q); // a^b mod q
	virtual ~RSACipher();
};

#endif /* RSACIPHER_H_ */
