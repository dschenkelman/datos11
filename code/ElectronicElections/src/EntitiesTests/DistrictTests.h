/*
 * DistrictTests.h
 *
 *  Created on: 28/09/2011
 *      Author: gonzalo
 */

#ifndef DISTRICTTESTS_H_
#define DISTRICTTESTS_H_

#include <string>
#include <iostream>
#include <string.h>
#include <stdlib.h>

class DistrictTests {
	void printResult(std::string testName, bool result);
public:
	DistrictTests();
	bool testGetSizeReturnsCorrectValue();
	bool testGetBytesReturnsCorrectValue();
	bool testSetBytes();
	void run();
	virtual ~DistrictTests();
};

#endif /* DISTRICTTESTS_H_ */
