/*
 * Block.h
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include "Record.h"
#include "RecordMethods.h"

// represents a block. format:
// BLOCK(contentSize(4 bytes), (registerSize(4 bytes), registerContent)*)
class Block {
	// -------------------FIELDS-------------------
	// object used to compare records
	RecordMethods* recordMethods;
	// max size of the block [bytes]
	int maxSize;
	// number of records occupied
	int occupiedRecords;
	// content of the block
	char* bytes;
	// amount of records in the block
	int recordCount;
	// max size of the records
	int recordSize;
	// position relative to the beginning of the block
	int currentRecord;
	// amount of bytes used to the status of records
	short flagBytes;
	// whether the block has overflown or not
	bool overflow;
	int findFirstFreeRecord();
	short getFlagByteFromRecordIndex(int recordIndex);
	// -------------------METHODS-------------------
public:
    bool hasNextRecord();
	//Block knows if it gets overflowed
	bool isOverflowed();
	void getsOverflow();
	Block(int size, int recordSize, RecordMethods* methods);
	bool isEmpty();
	bool isFull();
	int getRecordCount();
	void seekRecord(int recordNumber);
	char* getBytes();
	bool canInsertRecord();
	int findRecord(const char* key, Record* rec);
	void clear();
	void printContent();
	Record* getNextRecord(Record* r);
	Record* getRecord(Record* r);
	bool insertRecord(char* key, char* bytes);
	bool updateRecord(const char* key, char* bytes);
	bool removeRecord(const char* key);
	virtual ~Block();
};

#endif /* BLOCK_H_ */
