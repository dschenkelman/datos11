/*
 * ElectionsListTests.h
 *
 *  Created on: 30/09/2011
 *      Author: lejosdelcielo
 */

#ifndef ELECTIONSLISTTESTS_H_
#define ELECTIONSLISTTESTS_H_
#include <string>

class ElectionsListTests {
	void printResult(std::string testName, bool result);
public:
	ElectionsListTests();
	void run();
	bool testGetSize();
	bool testGetBytes();
	bool testSetBytes();
	virtual ~ElectionsListTests();
};

#endif /* ELECTIONSLISTTESTS_H_ */
