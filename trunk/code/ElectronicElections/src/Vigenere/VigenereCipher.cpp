/*
 * VigenereCipher.cpp
 *
 *  Created on: Nov 6, 2011
 *      Author: damian
 */

#include "VigenereCipher.h"

using namespace std;
#include <iostream>;
#include <string.h>

VigenereCipher::VigenereCipher() {
}

string VigenereCipher::encript(string & message, string & key)
{
	int crypted;
	string cryptedMessage("");
	int alphabethLength = 'Z' - 'A' + 1;

	int i=0, j=0;
	while ( i < (message.length()))
	{
		if (j == key.length())
			j = 0;

		if (!isalpha((message[i])))
		{
			cryptedMessage += message[i];
			i++;
			continue;
		}

		crypted = ((message[i] + key[j]) % alphabethLength) + 'A';

		cryptedMessage += (char)crypted;
		i++; j++;
	}

	return cryptedMessage;
}

string VigenereCipher::decript(string & cryptedMessage, string & key)
{
	int decrypted = 0;
	string message("");
	int alphabethLength = 'Z' - 'A' + 1;

	int i=0, j=0;
	while ( i < (cryptedMessage.length()))
	{
		if (j == key.length())
			j = 0;

		if (!isalpha(cryptedMessage[i]))
		{
			message += cryptedMessage[i];
			i++;
			continue;
		}

		decrypted = (cryptedMessage[i] - 'A') - key[j];
		while (decrypted < 'A')
		{
			decrypted += alphabethLength;
		}

		message += (char)decrypted;
		i++; j++;
	}

	return message;
}

VigenereCipher::~VigenereCipher() {
}
