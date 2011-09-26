/*
 * SequenceTreeBlock.h
 *
 *  Created on: Sep 25, 2011
 *      Author: damian
 */

#ifndef SEQUENCETREEBLOCK_H_
#define SEQUENCETREEBLOCK_H_

#include "TreeBlock.h"

class SequenceTreeBlock : public TreeBlock
{
private:
    void updateFreeSpace(int space);
protected:
	static const char NEXT_NODE_SIZE = sizeof(int);
	static const char RECORD_OFFSET =
			NEXT_NODE_SIZE +
			TreeBlock::LEVEL_SIZE +
			TreeBlock::FREE_SPACE_SIZE;
	virtual bool hasNextRecord();
public:
	SequenceTreeBlock(int size, RecordMethods* methods);
	virtual void updateInformation();
	virtual void clear();
	virtual bool canInsertRecord(int size);
	virtual bool insertRecord(const char* key, VariableRecord* rec);
	virtual bool insertSeparator(const char* key, short len);
	virtual UpdateResult updateRecord(const char* key, VariableRecord* rec);
	virtual void printContent();
	virtual bool removeRecord(const char* key);
	virtual bool removeSeparator(const char* key);
	virtual void forceInsert(VariableRecord *rec);
	virtual VariableRecord* getNextRecord(VariableRecord* r);
	virtual ~SequenceTreeBlock();
};

#endif /* SEQUENCETREEBLOCK_H_ */
