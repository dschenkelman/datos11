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

KeyManager::KeyManager(int keySize):fileExists(false)
{
	string privateKey = "./Files/Keys/key.pri";
	string publicKey = "./Files/Keys/key.pub";

	this->fileExists = this->file_exists(privateKey.c_str()) && this->file_exists(publicKey.c_str());
	this->keySize = keySize;

	if(this->fileExists)
	{
		ifstream privateKeysFile(privateKey.c_str(), ifstream::binary);
		ifstream publicKeysFile(publicKey.c_str(), ifstream::binary);
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

bool KeyManager::file_exists(const char * filename)
{
    if (FILE * file = fopen(filename, "r"))
    {
        fclose(file);
        return true;
    }
    return false;
}

bool KeyManager::generate()
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

		return true;
	}

	return false;
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

