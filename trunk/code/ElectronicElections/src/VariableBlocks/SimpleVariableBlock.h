/*
 * RLVBlock.h
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#ifndef SIMPLEVARIABLEBLOCK_H_
#define SIMPLEVARIABLEBLOCK_H_

#include "VariableRecord.h"
#include "RecordMethods.h"
#include "Constants.h"
#include "BaseVariableBlock.h"

// represents a block. format:
// SimpleVariableBlock(contentSize(4 bytes), (registerSize(4 bytes), registerContent)*)
class SimpleVariableBlock: public BaseVariableBlock
{
protected:
	virtual bool hasNextRecord();
public:
	SimpleVariableBlock(int size, RecordMethods* methods);
	// calculates the amount of records in the block
	// and the amount of free space available in it
	virtual void updateInformation();
	virtual void clear();
	virtual bool canInsertRecord(int size);
	virtual bool insertRecord(const char* key, VariableRecord* rec);
	virtual UpdateResult updateRecord(const char* key, VariableRecord* rec);
	virtual void printContent();
	virtual bool removeRecord(const char* key);
	virtual void forceInsert(VariableRecord *rec);
	virtual VariableRecord* getNextRecord(VariableRecord* r);
	virtual void positionAtBegin();
	virtual ~SimpleVariableBlock();
};

#endif /* SIMPLEVARIABLEBLOCK_H_ */
