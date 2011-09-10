/*
 * Block.h
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include "Record.h"

// represents a block. format:
// B(contentSize(4 bytes), (registerSize(4 bytes), registerContent)*)
class Block {
	// -------------------FIELDS-------------------

	// max size of the block [bytes]
	int maxSize;
	// occupied size of the block [bytes]
	int freeSpace;
	// content of the block
	char* bytes;
	// amount of records in the block
	int recordCount;
public:
	Block(int size);
	char* getBytes();
	int getFreeSpace();
	int getRecordCount();
	// calculates the amount of records in the block
	// and the amount of free space available in it
	void updateInformation();
	bool canInsertRecord(Record* rec);
	void insertRecord(Record* rec);
	virtual ~Block();
    void forceInsert(Record *rec);
};

#endif /* BLOCK_H_ */
