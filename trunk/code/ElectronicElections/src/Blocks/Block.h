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
class Block
{
	// -------------------FIELDS-------------------
	// whether the block has overflown or not
	bool overflow;
protected:
	// amount of records in the block
	int recordCount;
	// max size of the block [bytes]
	int maxSize;
	// number of records occupied
	int occupiedRecords;
	// size of the records
	int recordSize;
	// object used to compare records
	RecordMethods* recordMethods;
	// content of the block
	char* bytes;
	// position relative to the beginning of the block
	int currentRecord;
public:
	Block(int size, int recordSize, RecordMethods* methods);
	bool hasNextRecord();
	//Block knows if it gets overflowed
	bool isOverflowed();
	void getsOverflow();
	bool isEmpty();
	bool isFull();
	int getRecordCount();
	void seekRecord(int recordNumber);
	char* getBytes();
	int getRecordSize();
	bool canInsertRecord();
	int findRecord(const char* key, Record* rec);
	void clear();
	void printContent();
	Record* getNextRecord(Record* r);
	int getOccupiedRecords();
	virtual bool updateRecord(const char* key, char* bytes) = 0;
	virtual Record* getCurrentRecord(Record* r) = 0;
	virtual bool insertRecord(char* key, char* bytes) = 0;
	virtual bool removeRecord(const char* key) = 0;
	virtual ~Block();

};

#endif /* BLOCK_H_ */
