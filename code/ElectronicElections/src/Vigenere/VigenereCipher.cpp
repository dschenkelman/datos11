/*
 * VigenereCipher.cpp
 *
 *  Created on: Nov 6, 2011
 *      Author: damian
 */

#include "VigenereCipher.h"

using namespace std;
#include <iostream>;

VigenereCipher::VigenereCipher() {
}

string VigenereCipher::encript(string & message, string & key)
{
	int crypted;
	string cryptedMessage("");
	int alphabethLength = 126 - 32;	// 126 - 32 = 92

	int i=0, j=0;
	while ( i < (message.length()))
	{
		if (j == key.length())
			j = 0;
		int auxM = message[i] - 32;
		int auxK = key[j] - 32;

		crypted = (auxM + auxK) % alphabethLength;

		crypted += 32;
		cryptedMessage += (char)crypted;
		i++; j++;
	}

	return cryptedMessage;
}

string VigenereCipher::decript(string & message, string & key)
{
	return "";
}

VigenereCipher::~VigenereCipher() {
}