/*
 * KeyManagerTests.h
 *
 *  Created on: 17/11/2011
 *      Author: lejosdelcielo
 */

#ifndef KEYMANAGERTESTS_H_
#define KEYMANAGERTESTS_H_

#include "KeyManager.h"
#include <string>

class KeyManagerTests {
private:
	void printResult(std::string testName, bool result);
public:
	KeyManagerTests();
	void run();
	bool testGenerate();
	virtual ~KeyManagerTests();
};

#endif /* KEYMANAGERTESTS_H_ */
