/*
 * RSAAttackerTests.h
 *
 *  Created on: 08/11/2011
 *      Author: lejosdelcielo
 */

#ifndef RSAATTACKERTESTS_H_
#define RSAATTACKERTESTS_H_

#include <string>
#include <iostream>

using namespace std;

class RSAAttackerTests {
private:
	void printResult(std::string testName, bool result);
public:
	RSAAttackerTests();
	void run();
	bool testAttack();
	virtual ~RSAAttackerTests();
};

#endif /* RSAATTACKERTESTS_H_ */
