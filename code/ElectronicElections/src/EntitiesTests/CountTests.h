/*
 * CountTests.h
 *
 *  Created on: 05/10/2011
 *      Author: lejosdelcielo
 */

#ifndef COUNTTESTS_H_
#define COUNTTESTS_H_

#include <string>

class CountTests {
private:
	void printResult(std::string testName, bool result);
public:
	CountTests();
	void run();
	bool testGetSize();
	bool testGetKeySize();
	bool testGetBytes();
	bool testSetBytes();
	bool testGetKey();
	virtual ~CountTests();
};

#endif /* COUNTTESTS_H_ */
