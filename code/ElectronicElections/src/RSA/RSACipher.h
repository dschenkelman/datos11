/*
 * RSACipher.h
 *
 *  Created on: 06/11/2011
 *      Author: lejosdelcielo
 */

#ifndef RSACIPHER_H_
#define RSACIPHER_H_

#include "RSAKey.h"

class RSACipher {
public:
	RSACipher();
	static int64 productInverse(int64 n, int64 d);
	virtual ~RSACipher();
};

#endif /* RSACIPHER_H_ */
