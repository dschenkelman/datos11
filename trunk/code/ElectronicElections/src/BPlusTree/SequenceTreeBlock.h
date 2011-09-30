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
	static const char NEXT_NODE_SIZE = sizeof(int);
protected:
	virtual bool hasNextRecord();
public:
	static const char RECORD_OFFSET =
						NEXT_NODE_SIZE +
						TreeBlock::LEVEL_SIZE +
						TreeBlock::FREE_SPACE_SIZE;
	SequenceTreeBlock(int size, RecordMethods* methods);
	virtual void updateInformation();
	virtual void clear();
	virtual bool canInsertRecord(int size);
	virtual bool insertRecord(VariableRecord* keyRecord, VariableRecord* dataRecord);
	virtual UpdateResult updateRecord(const char* key, VariableRecord* rec);
	virtual void printContent();
	virtual bool removeRecord(const char* key);
	virtual void forceInsert(VariableRecord *rec);
	virtual void positionAtBegin();
	virtual void insertNodePointer(int index, int node);
	virtual void updateNodePointer(int index, int node);
	virtual void removeNodePointer(int index);
	virtual int getNodePointer(int index);
	virtual ~SequenceTreeBlock();
};

#endif /* SEQUENCETREEBLOCK_H_ */
