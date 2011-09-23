/*
 * TreeBlock.h
 *
 *  Created on: Sep 20, 2011
 *      Author: damian
 */

#ifndef TREEBLOCK_H_
#define TREEBLOCK_H_

#include "../Blocks/Block.h"

class TreeBlock: public Block
{
	static const int BLOCK_POINTER_SIZE = sizeof(int);
	static const int COUNT_SIZE = sizeof(int);
	static const int LEVEL_SIZE = sizeof(short);
	static const int RECORDS_OFFSET =
			BLOCK_POINTER_SIZE + COUNT_SIZE + LEVEL_SIZE;
	char nextBlockBytes;
	short level;
	int nextBlock;
	void updateOccupiedRecords(char diff);
	int retrieveOccupiedRecords();
	void loadMetadata();
	Record* getPreviousRecord(Record *r);
public:
	TreeBlock(int size, int recordSize, RecordMethods* methods);
	void setNextBlock(int blockNumber);
	int getNextBlock();
	short getLevel();
	void setLevel(short value);
	virtual bool updateRecord(const char* key, char* bytes);
	virtual Record* getCurrentRecord(Record* r);
	virtual bool insertRecord(char* key, char* bytes);
	virtual bool removeRecord(const char* key);
	virtual ~TreeBlock();
};

#endif /* TREEBLOCK_H_ */
