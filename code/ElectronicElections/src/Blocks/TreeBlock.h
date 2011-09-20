/*
 * TreeBlock.h
 *
 *  Created on: Sep 20, 2011
 *      Author: damian
 */

#ifndef TREEBLOCK_H_
#define TREEBLOCK_H_

#include "Block.h"

class TreeBlock: public Block
{
	static const int BLOCK_POINTER_SIZE = 4;
public:
	TreeBlock(int size, int recordSize, RecordMethods* methods);
	void setNextBlock(int blockNumber);
	virtual ~TreeBlock();
};

#endif /* TREEBLOCK_H_ */
