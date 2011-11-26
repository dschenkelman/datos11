/*
 * RSAAttacker.h
 *
 *  Created on: 08/11/2011
 *      Author: lejosdelcielo
 */

#ifndef RSAATTACKER_H_
#define RSAATTACKER_H_

#include <limits>
#include <vector>
#include "RSAKey.h"

using namespace std;

class RSAAttacker {
	int64 intMaxValue;
	void getPrimeNumbers(int64 n, int64* p, int64* q, int keySize);
public:
	RSAAttacker();
	RSAKey attack(RSAKey& publicKey, int keySize);
	virtual ~RSAAttacker();
};

#endif /* RSAATTACKER_H_ */
