/*
 * VigenereCipherTests.cpp
 *
 *  Created on: Nov 6, 2011
 *      Author: juanma
 */

#include "VigenereCipherTests.h"
#include "VigenereCipher.h"
#include <string>

VigenereCipherTests::VigenereCipherTests()
{
	// TODO Auto-generated constructor stub

}

void VigenereCipherTests::run()
{
	VigenereCipher vg;
	std::string aux("");
	vg.encript(aux,aux);
}

VigenereCipherTests::~VigenereCipherTests() {
	// TODO Auto-generated destructor stub
}
