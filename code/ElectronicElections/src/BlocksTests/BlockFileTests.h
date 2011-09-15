/*
 * BlockFileTests.h
 *
 *  Created on: Sep 15, 2011
 *      Author: damian
 */

#ifndef BLOCKFILETESTS_H_
#define BLOCKFILETESTS_H_

#include "./../Blocks/BlockFile.h"

class BlockFileTests
{
	BlockFile* file;
	BlockFile* removeFile;
public:
	BlockFileTests();
	void testInsert();
	void testUpdate();
	void testRemove();
	void testGet();
	void run();
	virtual ~BlockFileTests();
};

#endif /* BLOCKFILETESTS_H_ */
