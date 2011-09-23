/*
 * BlockFileTests.h
 *
 *  Created on: Sep 15, 2011
 *      Author: damian
 */

#ifndef BLOCKFILETESTS_H_
#define BLOCKFILETESTS_H_

#include "./../Blocks/BlockFile.h"

class HashBlockFileTests
{
	BlockFile* file;
	BlockFile* removeFile;
public:
	HashBlockFileTests();
	void testInsert();
	void testUpdate();
	void testRemove();
	void testGet();
	void run();
	virtual ~HashBlockFileTests();
};

#endif /* BLOCKFILETESTS_H_ */
