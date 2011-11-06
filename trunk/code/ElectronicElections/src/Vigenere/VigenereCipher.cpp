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
	int alphabethLength = 126 - 32;

	int i=0, j=0;
	while ( i < (message.length()))
	{
		if (j == key.length())
			j = 0;

		if (!isprint(message[i]))
		{
			cryptedMessage += message[i];
			i++;
			continue;
		}

		int auxM = message[i] - 32;
		int auxK = key[j] - 32;

		crypted = (auxM + auxK) % alphabethLength;

		crypted += 32;
		cryptedMessage += (char)crypted;
		i++; j++;
	}

	return cryptedMessage;
}

string VigenereCipher::decript(string & cryptedMessage, string & key)
{
	int decrypted;
	string message("");
	int alphabethLength = 126 - 32;

	int i=0, j=0;
	while ( i < (cryptedMessage.length()))
	{
		if (j == key.length())
			j = 0;

		if (!isprint(cryptedMessage[i]))
		{
			message += cryptedMessage[i];
			i++;
			continue;
		}

		int auxC = cryptedMessage[i] - 32;
		int auxK = key[j] - 32;

		decrypted = (auxC - auxK);
		if (decrypted < 0)
		{
			decrypted += alphabethLength;
		}

		decrypted = decrypted % alphabethLength;

		decrypted += 32;
		message += (char)decrypted;
		i++; j++;
	}

	return message;
}

VigenereCipher::~VigenereCipher() {
}
