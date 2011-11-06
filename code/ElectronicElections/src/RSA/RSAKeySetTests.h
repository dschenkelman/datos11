/*
 * RSAKeySetTests.h
 *
 *  Created on: 06/11/2011
 *      Author: lejosdelcielo
 */

#ifndef RSAKEYSETTESTS_H_
#define RSAKEYSETTESTS_H_
#include <string>

#include "RSAKeySet.h"
#include "RSAKey.h"
#include <iostream>
using namespace std;

#include <string>

using namespace std;

class RSAKeySetTests {
private:
	void printResult(std::string testName, bool result);
public:
	RSAKeySetTests();
	void run();
	bool testGeneration();
	static int64 negativeModulus(int64 base, int64 modulus);
	virtual ~RSAKeySetTests();
};

#endif /* RSAKEYSETTESTS_H_ */
