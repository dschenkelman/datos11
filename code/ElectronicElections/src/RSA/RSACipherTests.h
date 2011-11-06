/*
 * RSACipherTests.h
 *
 *  Created on: 06/11/2011
 *      Author: lejosdelcielo
 */

#ifndef RSACIPHERTESTS_H_
#define RSACIPHERTESTS_H_
#include <string>

#include "RSACipher.h"
#include <iostream>
using namespace std;

class RSACipherTests {
private:
	void printResult(std::string testName, bool result);
public:
	RSACipherTests();
	void run();
	bool testgenerateRelativelyPrimeNumbers();
	bool testProductInverse();
	virtual ~RSACipherTests();
};

#endif /* RSACIPHERTESTS_H_ */
