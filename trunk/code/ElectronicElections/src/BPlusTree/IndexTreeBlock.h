/*
 * IndexTreeBlock.h
 *
 *  Created on: Sep 27, 2011
 *      Author: juanma
 */

#ifndef INDEXTREEBLOCK_H_
#define INDEXTREEBLOCK_H_

#include "TreeBlock.h"

class IndexTreeBlock: public TreeBlock
{
	static const char FIRST_POINTER_POSITION = sizeof(int);
	static const char RECORD_OFFSET =
			FIRST_POINTER_POSITION +
				TreeBlock::LEVEL_SIZE +
				TreeBlock::FREE_SPACE_SIZE;
public:
	IndexTreeBlock(int size, RecordMethods* methods);
	virtual bool insertRecord(VariableRecord* keyRecord, VariableRecord* dataRecord);
	virtual ~IndexTreeBlock();
};

#endif /* INDEXTREEBLOCK_H_ */
