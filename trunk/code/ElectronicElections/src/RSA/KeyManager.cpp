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

		int lenN = strlen(stringN);

		char* partOne = new char[lenN/2];
		char* partTwo = new char[(lenN - lenN/2)];

		strncpy(partOne, stringN, lenN/2);
		strcpy(partTwo, stringN+(lenN/2));

		long longPartOne = atol(partOne);
		long longPartTwo = atol(partTwo);

//		this->privateKey.n = 0;
//		this->privateKey.n = (longPartOne << 32) | longPartTwo;
		this->privateKey.n = (long long) atol(stringN);

		int lenExp = strlen(stringExp);

		delete[] partOne;
		delete[] partTwo;

		partOne = new char[lenExp/2];
		partTwo = new char[(lenExp - lenExp/2)];

		strncpy(partOne, stringN, lenExp/2);
		strcpy(partTwo, stringN+(lenExp/2));

		longPartOne = 0, longPartTwo = 0;

		longPartOne = atol(partOne);
		longPartTwo = atol(partTwo);

//		this->privateKey.exp = 0;
//		this->privateKey.exp = (longPartOne << 32) | longPartTwo;
		this->privateKey.exp = (long long) atol(stringExp);

		delete[] partOne;
		delete[] partTwo;

		file.getline(line, MAX_LENGHT);

		stringN = strtok(line, ",");
		stringExp = strtok(NULL, ",");

		lenN = strlen(stringN);

		partOne = new char[lenN/2];
		partTwo = new char[(lenN - lenN/2)];

		strncpy(partOne, stringN, lenN/2);
		strcpy(partTwo, stringN+(lenN/2));

		longPartOne = atol(partOne);
		longPartTwo = atol(partTwo);

//		this->privateKey.n = 0;
//		this->privateKey.n = (longPartOne << 32) | longPartTwo;
		this->publicKey.n = (long long) atol(stringN);

		lenExp = strlen(stringExp);

		delete[] partOne;
		delete[] partTwo;

		partOne = new char[lenExp/2];
		partTwo = new char[(lenExp - lenExp/2)];

		strncpy(partOne, stringN, lenExp/2);
		strcpy(partTwo, stringN+(lenExp/2));

		longPartOne = 0, longPartTwo = 0;

		longPartOne = atol(partOne);
		longPartTwo = atol(partTwo);

//		this->privateKey.exp = 0;
//		this->privateKey.exp = (longPartOne << 32) | longPartTwo;
		this->publicKey.exp = (long long) atol(stringExp);

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

