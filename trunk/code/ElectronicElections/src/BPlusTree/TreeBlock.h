/*
 * TreeBlock.h
 *
 *  Created on: Sep 25, 2011
 *      Author: damian
 */

#ifndef TREEBLOCK_H_
#define TREEBLOCK_H_

#include "../VariableBlocks/BaseVariableBlock.h"

class TreeBlock: public BaseVariableBlock
{
protected:
	void updateFreeSpace(int space);
	virtual bool hasNextRecord() = 0;
	short level;
public:
	static const char LEVEL_SIZE = sizeof(short);
	static const char FREE_SPACE_SIZE = sizeof(int);
	TreeBlock(int size,  int pos, int rOffset, RecordMethods* methods);
	short getLevel();
	void setLevel(short value);
	virtual void updateInformation() = 0;
	virtual void clear() = 0;
	virtual bool canInsertRecord(int size) = 0;
	virtual bool insertRecord(VariableRecord* keyRecord, VariableRecord* dataRecord) = 0;
	virtual UpdateResult updateRecord(const char* key, VariableRecord* rec) = 0;
	virtual void printContent() = 0;
	virtual bool removeRecord(const char* key) = 0;
	virtual void forceInsert(VariableRecord *rec) = 0;
	VariableRecord* getNextRecord(VariableRecord* r);
	virtual void positionAtBegin();
	virtual void insertNodePointer(int index, int node) = 0;
	virtual void updateNodePointer(int index, int node) = 0;
	virtual void removeNodePointer(int index) = 0;
	virtual int getNodePointer(int index) = 0;
	virtual void setNextNode(int node) = 0;
	virtual int getNextNode() = 0;
	virtual bool isUnderflow() = 0;
	virtual ~TreeBlock();
};

#endif /* TREEBLOCK_H_ */
