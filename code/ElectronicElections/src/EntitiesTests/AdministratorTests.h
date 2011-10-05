/*
 * AdministratorTests.h
 *
 *  Created on: 04/10/2011
 *      Author: lejosdelcielo
 */

#ifndef ADMINISTRATORTESTS_H_
#define ADMINISTRATORTESTS_H_

#include <string>
#include <iostream>

class AdministratorTests {
private:
	void printResult(std::string testName, bool result);
public:
	AdministratorTests();
	void run();
	bool testGetSize();
	bool testGetKeySize();
	bool testGetBytes();
	bool testSetBytes();
	bool testGetKey();
	virtual ~AdministratorTests();
};

#endif /* ADMINISTRATORTESTS_H_ */
