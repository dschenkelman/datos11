/*
 * CandidateTests.h
 *
 *  Created on: 04/10/2011
 *      Author: lejosdelcielo
 */

#ifndef CANDIDATETESTS_H_
#define CANDIDATETESTS_H_

#include <string>
#include <iostream>

using namespace std;

class CandidateTests {
private:
	void printResult(std::string testName, bool result);
public:
	CandidateTests();
	void run();
	bool testGetSize();
	bool testGetBytes();
	bool testSetBytes();
	virtual ~CandidateTests();
};

#endif /* CANDIDATETESTS_H_ */
