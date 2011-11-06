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
	string tempMessage("Lorem ipsum dolor sit ametconsecteturadipisicingeliseddoeiusmodtemporincididuntutlaboreetdoloremagnaaliquDuisauteiruredolorinreprehenderitinvoluptatevelitessecillumdoloreeufugiatnullapariatur");
	string tempKey("jihgfedcba");	//long: 10
	int crypted;
	string cryptedMessage("");
	int alphabethLength = 126 - 32;	// 126 - 32 = 92

	int i=0, j=0;
	while ( i < (tempMessage.length()))
	{
		if (j == tempKey.length())
			j = 0;
		int auxM = tempMessage[i] - 32;
		int auxK = tempKey[j] - 32;

		crypted = (auxM + auxK) % alphabethLength;

		crypted += 32;
		cryptedMessage += (char)crypted;
		i++; j++;
	}
	cout << "Vigenere Encryption" << endl << "Message: " << tempMessage << endl << "Key: " << tempKey << endl;
	cout << "Crypted Message: " << cryptedMessage << endl;
}

string VigenereCipher::decript(string & message, string & key)
{
	return "";
}

VigenereCipher::~VigenereCipher() {
}
