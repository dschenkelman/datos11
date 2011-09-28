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
	static const char SEPARATOR_HEADER_SIZE = sizeof(short);
	static const char RECORD_OFFSET =
			FIRST_POINTER_POSITION +
				TreeBlock::LEVEL_SIZE +
				TreeBlock::FREE_SPACE_SIZE;
	// Amount of keys per block (child number is the same plus 1)
	static const short MAX_KEYS = 4;
	// Position of the first Pointer (to a child) relative to the beginning of the block
	int firstPointerPosition;
public:
	IndexTreeBlock(int size, RecordMethods* methods);
	virtual void updateInformation() = 0;
	virtual void clear() = 0;
	virtual bool canInsertRecord(int size) = 0;
	virtual bool insertRecord(VariableRecord* keyRecord, VariableRecord* dataRecord);
	virtual bool insertSeparator(const char* key, short len);
	virtual UpdateResult updateRecord(const char* key, VariableRecord* rec) = 0;
	virtual void printContent() = 0;
	virtual bool removeRecord(const char* key) = 0;
	virtual bool removeSeparator(const char* key) = 0;
	virtual void forceInsert(VariableRecord *rec) = 0;
	virtual VariableRecord* getNextRecord(VariableRecord* r) = 0;
	virtual ~IndexTreeBlock();
};

#endif /* INDEXTREEBLOCK_H_ */
