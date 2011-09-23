/*
 * HashBlock.h
 *
 *  Created on: Sep 22, 2011
 *      Author: damian
 */

#ifndef HASHBLOCK_H_
#define HASHBLOCK_H_

#include "../Blocks/Block.h"

class HashBlock: public Block
{
	// amount of bytes used to the status of records
	short flagBytes;
	// -------------------METHODS-------------------
	int findFirstFreeRecord();
	short getFlagByteFromRecordIndex(int recordIndex);
public:
	HashBlock(int size, int recordSize, RecordMethods* methods);
	virtual bool updateRecord(const char* key, char* bytes);
	virtual Record* getCurrentRecord(Record* r);
	virtual bool insertRecord(char* key, char* bytes);
	virtual bool removeRecord(const char* key);
	bool insertInCurrentRecord(char* key, char* bytes);
	virtual ~HashBlock();
};

#endif /* HASHBLOCK_H_ */
