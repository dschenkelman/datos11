/*
 * TreeBlockFileTests.h
 *
 *  Created on: Sep 20, 2011
 *      Author: gabriel
 */

#ifndef HASHTEST_H_
#define HASHTEST_H_

#include "HashBlockFile.h"
#include "DistrictHashingFunction.h"
#include "../BlocksTests/CustomerMethods.h"

class HashTest
{
	HashBlockFile* file;
	DistrictHashingFunction* dhashf;
	CustomerMethods* cm;

public:
	HashTest();
	void testLoadHashwithoutValidation();
	void testInsert();
	void testEmptyBlock(int blockNumber);
	void testGetRecord();
	void testUpdateRecord();
	void testRemove();
	void run();
	virtual ~HashTest();
};

#endif //HASHTEST_H_
