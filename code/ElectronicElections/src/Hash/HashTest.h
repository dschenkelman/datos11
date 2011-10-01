/*
 * TreeBlockFileTests.h
 *
 *  Created on: Sep 20, 2011
 *      Author: gabriel
 */

#ifndef HASHTEST_H
#define HASHTEST_H

#include "HashBlockFile.h"

class HashTest
{
	HashBlockFile* file;

public:
	HashTest();
	void testInsert();
	void testEmptyBlock(int blockNumber);
	//void testGetRecord();
	void testRemove();
	void run();
	virtual ~HashTest();
};
#endif HASHTEST_H
