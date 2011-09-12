/*
 * Block.h
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include "Record.h"
#include "RecordComparer.h"

// represents a block. format:
// BLOCK(contentSize(4 bytes), (registerSize(4 bytes), registerContent)*)
class Block {
	// -------------------FIELDS-------------------
	// object used to compare records
	RecordComparer* recordComparer;
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
    void copyBlock(const Block & other);

	// -------------------METHODS-------------------
    bool hasNextRecord();
    Record* getNextRecord(Record* r);
    inline int getOccupiedSize();
public:
	Block(int size, RecordComparer* comparer);
	Block(Block& other);
	Block& operator=(const Block& other);
	char* getBytes();
	int getFreeSpace();
	int getRecordCount();
	// calculates the amount of records in the block
	// and the amount of free space available in it
	void updateInformation();
	bool findRecord(char* key);
	void clear();
	bool canInsertRecord(int size);
	void insertRecord(Record* rec);
	virtual ~Block();
    void forceInsert(Record *rec);
};

#endif /* BLOCK_H_ */
