/*
 * TreeBlockFileTests.h
 *
 *  Created on: Sep 20, 2011
 *      Author: damian
 */

#ifndef TREEBLOCKFILETESTS_H_
#define TREEBLOCKFILETESTS_H_

#include "./../Blocks/TreeBlockFile.h"

class TreeBlockFileTests
{
	TreeBlockFile* file;
public:
	TreeBlockFileTests();
	void testInsert();
	void run();
	virtual ~TreeBlockFileTests();
};

#endif /* TREEBLOCKFILETESTS_H_ */
