/*
 * HashBlock.h
 *
 *  Created on: Sep 22, 2011
 *      Author: damian
 */

#ifndef HASHBLOCK_H_
#define HASHBLOCK_H_

#include "../VariableBlocks/SimpleVariableBlock.h"

class HashBlock: public SimpleVariableBlock
{
	// indicates if the block is overflow and gives the overflow block address in the overflow file
	char overflowBlockPointer;
	// -------------------METHODS-------------------
	int findFirstFreeRecord();
	short getFlagByteFromRecordIndex(int recordIndex);
public:
	HashBlock(int size, int recordSize, RecordMethods* methods);
	virtual VariableRecord* getCurrentRecord(VariableRecord* r);
	/*
	virtual bool insertRecord(char* key, char* bytes);
	virtual bool removeRecord(const char* key);
	virtual bool updateRecord(const char* key, char* bytes);
	*/
	bool insertInCurrentRecord(char* key, char* bytes);
	virtual ~HashBlock();
};

#endif /* HASHBLOCK_H_ */
