/*
 * HashBlock.h
 *
 *  Created on: Sep 22, 2011
 *      Author: damian
 */

#ifndef HASHBLOCK_H_
#define HASHBLOCK_H_

#include "../VariableBlocks/BaseVariableBlock.h"

class HashBlock: public BaseVariableBlock
{
	// -------------------METHODS-------------------
	virtual bool hasNextRecord();

public:
	HashBlock(int size, RecordMethods* methods);
	virtual void updateInformation();
	virtual VariableRecord* getNextRecord(VariableRecord* r);
	virtual void clear();
	virtual void printContent();
	int getOverflowedBlock();
	virtual void forceInsert(VariableRecord *rec);
	virtual bool insertRecord(const char* key, VariableRecord* record);
	virtual bool removeRecord(const char* key);
	virtual UpdateResult updateRecord(const char* key, VariableRecord* record);

	virtual ~HashBlock();
};

#endif /* HASHBLOCK_H_ */
