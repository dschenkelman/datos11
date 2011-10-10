/*
 * HashBlockFile.h
 *
 *  Created on: Sep 22, 2011
 *      Author: damian
 */

#ifndef HASHBLOCKFILE_H_
#define HASHBLOCKFILE_H_

#include "../VariableBlocks/SimpleVariableBlockFile.h"
#include "HashingFunction.h"
#include "HashBlock.h"
#include <fstream>

class HashBlockFile : BaseVariableBlockFile
{
	HashBlock* currentBlock;
	SimpleVariableBlockFile* overflowFile;
	// flag for having written in overflow file.
	bool ovflowBlockUsed;
	bool hashBlockUsed;
	int totalBlocks;
	HashingFunction* hashingFunction;
	void initializefile();
	int fakeHashFunction(const char* key);
	int findInOverflowBlocks(const char* key, VariableRecord** record, bool getFlag);
	int getAvailableOverflowBlock(const char* key, VariableRecord* record);

public:
	HashBlockFile(std::string name, int bSize, RecordMethods* methods, HashingFunction* hashFunction, int blockamount, bool createNew);
	virtual HashBlock* getCurrentBlock();
	void loadRecord(const char* key, VariableRecord* record);
	bool insertRecord(const char* key, VariableRecord* record);
	bool removeRecord(const char* key);
	bool getRecord(const char* key, VariableRecord** record);
	bool updateRecord(const char* key, VariableRecord* record);
	virtual void printContent();
	virtual void loadBlock(int blockNumber);
	virtual void saveBlock();
	virtual ~HashBlockFile();
};

#endif /* HASHBLOCKFILE_H_ */
