/*
 * VoterTests.h
 *
 *  Created on: 07/10/2011
 *      Author: lejosdelcielo
 */

#ifndef VOTERTESTS_H_
#define VOTERTESTS_H_

#include <string>

using namespace std;

class VoterTests {
private:
	void printResult(std::string testName, bool result);
public:
	VoterTests();
	void run();
	bool testGetSize();
	bool testGetKeySize();
	bool testGetBytes();
	bool testSetBytes();
	bool testGetKey();
	virtual ~VoterTests();
};

#endif /* VOTERTESTS_H_ */
