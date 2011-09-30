/*
 * TreeTests.h
 *
 *  Created on: Sep 30, 2011
 *      Author: damian
 */

#ifndef TREETESTS_H_
#define TREETESTS_H_

#include <string>

class TreeTests
{
	void printResult(std::string testName, bool result);
public:
	TreeTests();
	void run();
	bool testInsertInEmptyTreeWorksCorrectly();
	bool testInsertInRootWithOverflowCreatesTwoLeafs();
	virtual ~TreeTests();
};

#endif /* TREETESTS_H_ */
