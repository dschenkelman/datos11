/*
 * TreeBlockFileTests.h
 *
 *  Created on: Sep 20, 2011
 *      Author: gabriel
 *

#ifndef HASHTEST_H_
#define HASHTEST_H_

#include "../Hash/HashBlockFile.h"

class HashTest
{
	Hash* hash;
	HashBlockFile* file;

public:
	HashTest();
	void testInsert();
	void testGetRecord();
	void testRemove();
	void run();
	virtual ~HashTest();
};

#endif /* HASHTEST_H_ */
