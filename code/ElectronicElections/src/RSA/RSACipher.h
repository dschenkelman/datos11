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
	vector<int64> generateRelativelyPrimeNumbers(int64 number, int keySize);
public:
	RSACipher();
	int64 GCD(int64 numberOne, int64 numberTwo);
	int getChunkSize(int64 n);
	int64 getRelativelyPrimeNumber(int64 number, int keySize);
	static int64 productInverse(int64 n, int64 d);
	static int64 modularExponentiation(int64 a, int64 b, int64 q); // a^b mod q
	void cipherMessage(char* message, int64 expKey, int64 n, char* cipheredMessage, int64 messageLen);
	void decryptMessage(char* cipheredMessage, int64 expKey, int64 n, char* decryptedMessage, int64 messageLen);
	virtual ~RSACipher();

};

int64 mul64modulingMod(int64 a, int64 b, int64 c);

#endif /* RSACIPHER_H_ */
