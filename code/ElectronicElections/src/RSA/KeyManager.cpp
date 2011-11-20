/*
 * KeyManager.cpp
 *
 *  Created on: 17/11/2011
 *      Author: lejosdelcielo
 */

#include "KeyManager.h"
#include "RSAKeySet.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <sstream>

using namespace std;

KeyManager::KeyManager(int keySize)
{
	string fileName = "./Files/Keys/key.pri";
	ifstream privateKeysFile(fileName.c_str(), ifstream::binary);

	fileName = "./Files/Keys/key.pub";
	ifstream publicKeysFile(fileName.c_str(), ifstream::binary);

	this->fileExists = privateKeysFile.is_open() && publicKeysFile.is_open();
	this->keySize = keySize;

	if(this->fileExists)
	{
		char buffer[sizeof(int64)];

		privateKeysFile.read(buffer, sizeof(int64));
		memcpy(&(this->privateKey.n), buffer, sizeof(int64));

		privateKeysFile.read(buffer, sizeof(int64));
		memcpy(&(this->privateKey.exp), buffer, sizeof(int64));

		privateKeysFile.close();

		publicKeysFile.read(buffer, sizeof(int64));
		memcpy(&(this->publicKey.n), buffer, sizeof(int64));

		publicKeysFile.read(buffer, sizeof(int64));
		memcpy(&(this->publicKey.exp), buffer, sizeof(int64));

		publicKeysFile.close();
	}
}

void KeyManager::generate()
{
	if(!this->fileExists)
	{
		RSAKeySet rsaKeySet(this->keySize);
		this->privateKey = rsaKeySet.getPrivateKey();
		this->publicKey = rsaKeySet.getPublicKey();

		string fileName = "./Files/Keys/key.pri";
		ofstream privateKeysFile(fileName.c_str(), ofstream::binary);

		char buffer[sizeof(int64)];

		memcpy(buffer, &(this->privateKey.n), sizeof(int64));
		privateKeysFile.write(buffer, sizeof(int64));

		memcpy(buffer, &(this->privateKey.exp), sizeof(int64));
		privateKeysFile.write(buffer, sizeof(int64));

		privateKeysFile.close();

		fileName = "./Files/Keys/key.pub";

		ofstream publicKeysFile(fileName.c_str(), ofstream::binary);

		memcpy(buffer, &(this->publicKey.n), sizeof(int64));
		publicKeysFile.write(buffer, sizeof(int64));

		memcpy(buffer, &(this->publicKey.exp), sizeof(int64));
		publicKeysFile.write(buffer, sizeof(int64));

		publicKeysFile.close();
	}
}

RSAKey KeyManager::getPrivateKey()
{
	return this->privateKey;
}

RSAKey KeyManager::getPublicKey()
{
	return this->publicKey;
}

KeyManager::~KeyManager()
{
}

