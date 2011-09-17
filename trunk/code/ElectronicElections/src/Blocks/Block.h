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

enum UpdateResult { NOT_FOUND, UPDATED, INSUFFICIENT_SPACE };

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

	bool overflow;
    
	//void copyBlock(const Block & other);

	// -------------------METHODS-------------------
public:
    bool hasNextRecord();
    Record* getNextRecord(Record* r);
	Record* getRecord(Record* r);
    inline int getOccupiedSize();
    inline int getOccupiedRecordNumbers();
	//Block knows if it gets overflowed
	bool isOverflowed();
	void getsOverflow();
	Block(int size, int recordSize, RecordMethods* methods);
	Block(Block& other);
	Block& operator=(const Block& other);
	
	bool isEmpty();
	bool isFull();
	int getRecordCount();
	void seekRecord(int recordNumber);

	char* getBytes();
	// calculates the amount of records in the block
	// and the amount of free space available in it
	void updateInformation();
	bool canInsertRecord(int size);
	int findRecord(const char* key, Record** rec);
	void clear();
	// position to nay record on the block, and then you may insert the record in there.
	void positionToRecord();
	bool insertRecord(Record* rec);
	UpdateResult updateRecord(const char* key, Record* rec);
	bool removeRecord(const char* key);
	
	void printContent();
	virtual ~Block();
    //void forceInsert(Record *rec);
};

#endif /* BLOCK_H_ */
