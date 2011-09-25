/*
 * RLVBlockFileTests.h
 *
 *  Created on: Sep 15, 2011
 *      Author: damian
 */

#ifndef RLVBLOCKFILETESTS_H_
#define RLVBLOCKFILETESTS_H_

#include "./../VariableBlocks/SimpleVariableBlockFile.h"

class SimpleVariableBlockFileTests
{
	SimpleVariableBlockFile* file;
	SimpleVariableBlockFile* removeFile;
public:
	SimpleVariableBlockFileTests();
	void testInsert();
	void testUpdate();
	void testRemove();
	void testGet();
	void run();
	virtual ~SimpleVariableBlockFileTests();
};

#endif /* RLVBLOCKFILETESTS_H_ */
