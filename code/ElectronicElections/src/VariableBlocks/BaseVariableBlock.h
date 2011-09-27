/*
 * BaseVariableBlock.h
 *
 *  Created on: Sep 25, 2011
 *      Author: damian
 */

#ifndef BASEVARIABLEBLOCK_H_
#define BASEVARIABLEBLOCK_H_

#include "VariableRecord.h"
#include "RecordMethods.h"

enum UpdateResult { NOT_FOUND, UPDATED, INSUFFICIENT_SPACE };

class BaseVariableBlock
{
protected:
	// max size of the block [bytes]
	int maxSize;
	// position relative to the beginning of the block
	int position;
	// position of the first record relative to the beginning of the block
	int recordsOffset;
	// object used to compare records
	RecordMethods* recordMethods;
	// occupied size of the block [bytes]
	int freeSpace;
	// content of the block
	char* bytes;
	virtual bool hasNextRecord() = 0;
    int getOccupiedSize();
public:
	BaseVariableBlock(int size, int pos, int rOffset, RecordMethods* methods);
	char* getBytes();
	int getSize();
	int getFreeSpace();
	int getRecordCount();
	int findRecord(const char* key, VariableRecord** rec);
	virtual void updateInformation() = 0;
	virtual void clear() = 0;
	virtual bool canInsertRecord(int size) = 0;
	virtual bool insertRecord(const char* key, VariableRecord* rec) = 0;
	virtual UpdateResult updateRecord(const char* key, VariableRecord* rec) = 0;
	virtual void printContent() = 0;
	virtual bool removeRecord(const char* key) = 0;
	virtual void forceInsert(VariableRecord *rec) = 0;
	virtual VariableRecord* getNextRecord(VariableRecord* r) = 0;
	virtual void positionAtBegin();
	virtual ~BaseVariableBlock();
};

#endif /* BASEVARIABLEBLOCK_H_ */
