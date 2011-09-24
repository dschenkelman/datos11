/*
 * RLVBlock.h
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#ifndef RLVBLOCK_H_
#define RLVBLOCK_H_

#include "VariableRecord.h"
#include "./../Blocks/RecordMethods.h"
#include "./../Blocks/Constants.h"

enum UpdateResult { NOT_FOUND, UPDATED, INSUFFICIENT_SPACE };

// represents a block. format:
// RLVBlock(contentSize(4 bytes), (registerSize(4 bytes), registerContent)*)
class RLVBlock{
	// -------------------FIELDS-------------------
	// object used to compare records
	RecordMethods* recordMethods;
	// max size of the block [bytes]
	int maxSize;
	// occupied size of the block [bytes]
	int freeSpace;
	// content of the block
	char* bytes;
	// amount of records in the block
	int recordCount;
	// position relative to the beginning of the block
	int position;
    void copyBlock(const RLVBlock & other);

	// -------------------METHODS-------------------
    bool hasNextRecord();
    VariableRecord* getNextRecord(VariableRecord* r);
    inline int getOccupiedSize();

public:
	RLVBlock(int size, RecordMethods* methods);
	RLVBlock(RLVBlock& other);
	RLVBlock& operator=(const RLVBlock& other);
	char* getBytes();
	int getFreeSpace();
	int getRecordCount();
	// calculates the amount of records in the block
	// and the amount of free space available in it
	void updateInformation();
	int findRecord(const char* key, VariableRecord** rec);
	void clear();
	bool canInsertRecord(int size);
	bool insertRecord(VariableRecord* rec);
	UpdateResult updateRecord(const char* key, VariableRecord* rec);
	void printContent();
	bool removeRecord(const char* key);
	virtual ~RLVBlock();
    void forceInsert(VariableRecord *rec);
};

#endif /* RLVBLOCK_H_ */
