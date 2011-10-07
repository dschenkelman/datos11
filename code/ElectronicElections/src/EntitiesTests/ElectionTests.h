/*
 * ElectionTests.h
 *
 *  Created on: 06/10/2011
 *      Author: lejosdelcielo
 */

#ifndef ELECTIONTESTS_H_
#define ELECTIONTESTS_H_

#include <string>

using namespace std;

class ElectionTests {
private:
	void printResult(std::string testName, bool result);
public:
	ElectionTests();
	void run();
	bool testGetSize();
	bool testGetKeySize();
	bool testGetBytes();
	bool testSetBytes();
	bool testGetKey();
	virtual ~ElectionTests();
};

#endif /* ELECTIONTESTS_H_ */
