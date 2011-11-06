/*
 * ValidationTests.h
 *
 *  Created on: 06/11/2011
 *      Author: lejosdelcielo
 */

#ifndef VALIDATIONTESTS_H_
#define VALIDATIONTESTS_H_

#include <iostream>
#include <string>

using namespace std;

class ValidationTests {
private:
	void printResult(std::string testName, bool result);
public:
	ValidationTests();
	void run();
	bool testValidateDate();
	virtual ~ValidationTests();
};

#endif /* VALIDATIONTESTS_H_ */
