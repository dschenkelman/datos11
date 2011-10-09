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
	// Position of the first pointer (to a child node) relative to the beginning of the block
	int nodesPosition;
	void storeNodesPosition();
	void loadNodesPosition();
protected:
	virtual bool hasNextRecord();
public:
	static const char NODE_POINTER_SIZE = sizeof(int);
	static const char RECORD_OFFSET =
			FIRST_POINTER_POSITION +
				TreeBlock::LEVEL_SIZE +
				TreeBlock::FREE_SPACE_SIZE;
	IndexTreeBlock(int size, RecordMethods* methods, bool existing);
	virtual void updateInformation();
	virtual void clear();
	virtual bool canInsertRecord(int size);
	virtual bool insertRecord(VariableRecord* keyRecord, VariableRecord* dataRecord);
	virtual UpdateResult updateRecord(const char* key, VariableRecord* rec);
	virtual void printContent();
	virtual bool removeRecord(const char* key);
	virtual void forceInsert(VariableRecord *rec);
	virtual void insertNodePointer(int index, int node);
	virtual void updateNodePointer(int index, int node);
	virtual void removeNodePointer(int index);
	virtual int getNodePointer(int index);
	virtual void setNextNode(int node);
	virtual int getNextNode();
	virtual VariableRecord* popFirst();
	virtual VariableRecord* popLast();
	virtual ~IndexTreeBlock();
};

#endif /* INDEXTREEBLOCK_H_ */
