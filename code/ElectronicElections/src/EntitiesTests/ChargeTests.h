/*
 * ChargeTests.h
 *
 *  Created on: 06/10/2011
 *      Author: lejosdelcielo
 */

#ifndef CHARGETESTS_H_
#define CHARGETESTS_H_

#include <string>

class ChargeTests {
private:
	void printResult(std::string testName, bool result);
public:
	ChargeTests();
	void run();
	bool testGetSize();
	bool testGetKeySize();
	bool testGetBytes();
	bool testSetBytes();
	bool testGetKey();
	virtual ~ChargeTests();
};

#endif /* CHARGETESTS_H_ */
