/*
 * VigenereCipherTests.cpp
 *
 *  Created on: Nov 6, 2011
 *      Author: juanma
 */

#include "VigenereCipherTests.h"
#include "VigenereCipher.h"
#include <string>
#include <iostream>

using namespace std;

VigenereCipherTests::VigenereCipherTests()
{
	// TODO Auto-generated constructor stub

}

void VigenereCipherTests::run()
{
	VigenereCipher vg;
	string tempMessage("Lorem ipsum dolor sit ametconsecteturadipisicingeliseddoeiusmodtemporincididuntutlaboreetdoloremagnaaliquDuisauteiruredolorinreprehenderitinvoluptatevelitessecillumdoloreeufugiatnullapariatur");
	string tempKey("jihgfedcba");	//long: 10

	cout << "Vigenere Encryption" << endl << "Message: " << tempMessage << endl << "Key: " << tempKey << endl;

	string cryptedMessage = vg.encript(tempMessage,tempKey);
	cout << "Crypted Message: " << cryptedMessage << endl;

	string decryptedMessage = vg.decript(cryptedMessage,tempKey);
	cout << endl <<"Decrypted Message: " << decryptedMessage;
}

VigenereCipherTests::~VigenereCipherTests() {
	// TODO Auto-generated destructor stub
}
