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
	// amount of records in the block
	int recordCount;

	// whether the block has overflown or not
	bool overflow;
protected:
	// amount of bytes used to the status of records
	short flagBytes;
	// max size of the block [bytes]
	int maxSize;
	// number of records occupied
	int occupiedRecords;
	// size of the records
	int recordSize;
	// object used to compare records
	RecordMethods* recordMethods;
	char nextBlockBytes;
	// content of the block
	char* bytes;
	// position relative to the beginning of the block
	int currentRecord;
	// -------------------METHODS-------------------
	int findFirstFreeRecord();
	short getFlagByteFromRecordIndex(int recordIndex);
public:
    bool hasNextRecord();
	//Block knows if it gets overflowed
	bool isOverflowed();
	void getsOverflow();
	Block(int size, int recordSize, RecordMethods* methods, char nextBlockBytes = 0);
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
	Record* getCurrentRecord(Record* r);
	virtual bool insertRecord(char* key, char* bytes);
	bool updateRecord(const char* key, char* bytes);
	bool removeRecord(const char* key);
	int getOccupiedRecords();
	virtual ~Block();
};

#endif /* BLOCK_H_ */
