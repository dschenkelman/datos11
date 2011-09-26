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
	virtual bool insertRecord(const char* key, VariableRecord* rec) = 0;
	virtual bool insertSeparator(const char* key, short len) = 0;
	virtual UpdateResult updateRecord(const char* key, VariableRecord* rec) = 0;
	virtual void printContent() = 0;
	virtual bool removeRecord(const char* key) = 0;
	virtual bool removeSeparator(const char* key) = 0;
	virtual void forceInsert(VariableRecord *rec) = 0;
	virtual VariableRecord* getNextRecord(VariableRecord* r) = 0;
	virtual ~TreeBlock();
};

#endif /* TREEBLOCK_H_ */
