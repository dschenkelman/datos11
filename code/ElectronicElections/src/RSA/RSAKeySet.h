/*
 * RSAKeySet.h
 *
 *  Created on: 06/11/2011
 *      Author: alejandro
 */

#ifndef RSAKEYSET_H_
#define RSAKEYSET_H_

#include "RSAKey.h"

class RSAKeySet {
private:
	RSAKey privateKey;
	RSAKey publicKey;
public:
	int64 phi; // for testing purposes
	RSAKeySet();
	RSAKey getPrivateKey();
	RSAKey getPublicKey();
	virtual ~RSAKeySet();
};

#endif /* RSAKEYSET_H_ */
