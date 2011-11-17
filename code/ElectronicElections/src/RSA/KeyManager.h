/*
 * KeyManager.h
 *
 *  Created on: 17/11/2011
 *      Author: lejosdelcielo
 */

#ifndef KEYMANAGER_H_
#define KEYMANAGER_H_

#include "RSAKey.h"

const int MAX_LENGHT = 500;

class KeyManager {
private:
	RSAKey privateKey;
	RSAKey publicKey;
	bool fileExists;
public:
	KeyManager();
	void generate();
	RSAKey getPrivateKey();
	RSAKey getPublicKey();
	virtual ~KeyManager();
};

#endif /* KEYMANAGER_H_ */
