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
	ifstream file;
	string fileName = "./Files/Keys/keys.txt";
	file.open(fileName.c_str(), ios::in);

	this->fileExists = file.is_open();
	this->keySize = keySize;

	if(this->fileExists)
	{
		char* stringN;
		char* stringExp;

		char line[MAX_LENGHT];
		file.getline(line, MAX_LENGHT);

		stringN = strtok(line, ",");
		stringExp = strtok(NULL, ",");

		this->privateKey.n = (long long) atol(stringN);
		this->privateKey.exp = (long long) atol(stringExp);

		file.getline(line, MAX_LENGHT);

		stringN = strtok(line, ",");
		stringExp = strtok(NULL, ",");

		this->publicKey.n = (int64) atol(stringN);
		this->publicKey.exp = (int64) atol(stringExp);

		file.close();
	}
}

void KeyManager::generate()
{
	if(!this->fileExists)
	{
		RSAKeySet rsaKeySet(this->keySize);
		this->privateKey = rsaKeySet.getPrivateKey();
		this->publicKey = rsaKeySet.getPublicKey();
		stringstream publicStream, privateStream;
		publicStream << this->publicKey.n << "," << this->publicKey.exp;
		privateStream << this->privateKey.n << "," << this->privateKey.exp;

		ofstream file;

		string fileName = "./Files/Keys/keys.txt";
		file.open(fileName.c_str(), ios::out);

		file.write(privateStream.str().c_str(), privateStream.str().size());

		string endLine = "\n";
		file.write(endLine.c_str(), endLine.size());

		file.write(publicStream.str().c_str(), publicStream.str().size());

		file.close();
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

