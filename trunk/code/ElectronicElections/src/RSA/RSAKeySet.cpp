/*
 * RSAKeySet.cpp
 *
 *  Created on: 06/11/2011
 *      Author: alejandro
 */

#include "RSAKeySet.h"
#include "RSAKey.h"
#include "RSACipher.h"
#include "PrimeGenerator.h"
#include <limits>

RSAKeySet::RSAKeySet() {
	RSAKeySet(8);
}

RSAKeySet::RSAKeySet(int keySize) {
	PrimeGenerator pg(pow(2, keySize/2));
	int64 p = 0;
	int64 q = 0;
	pg.getRandomWithMinimum(10000, &p, &q);
	int64 n = p*q;
	cout << sizeof(p) << "|" << sizeof(q) << "|" << sizeof(n) << "|"<<endl;
	phi = (p-1)*(q-1);
	RSACipher rcac;
	int64 d = rcac.getRelativelyPrimeNumber(phi);
	if (rcac.GCD(d, phi)!=1) cout << "WARNING: error generando RSA"<<endl;
	int64 e = RSACipher::productInverse(phi, d);
	cout << " p " << p << " q " << q << " n " << n << " phi " << phi << " d " << d << " e " << e << endl;
	privateKey.exp = d;
	privateKey.n = n;
	publicKey.exp = e;
	publicKey.n = n;
}

RSAKey RSAKeySet::getPrivateKey() {
	return privateKey;
}

RSAKey RSAKeySet::getPublicKey() {
	return publicKey;
}

RSAKeySet::~RSAKeySet() {
}
