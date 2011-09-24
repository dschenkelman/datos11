/*
 * RLVBlockFileTests.h
 *
 *  Created on: Sep 15, 2011
 *      Author: damian
 */

#ifndef RLVBLOCKFILETESTS_H_
#define RLVBLOCKFILETESTS_H_

#include "./../RLVBlocks/RLVBlockFile.h"

class RLVBlockFileTests
{
	RLVBlockFile* file;
	RLVBlockFile* removeFile;
public:
	RLVBlockFileTests();
	void testInsert();
	void testUpdate();
	void testRemove();
	void testGet();
	void run();
	virtual ~RLVBlockFileTests();
};

#endif /* RLVBLOCKFILETESTS_H_ */
