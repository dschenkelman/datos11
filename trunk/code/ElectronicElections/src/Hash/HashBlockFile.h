/*
 * HashBlockFile.h
 *
 *  Created on: Sep 22, 2011
 *      Author: damian
 */

#ifndef HASHBLOCKFILE_H_
#define HASHBLOCKFILE_H_

#include "../VariableBlocks/SimpleVariableBlockFile.h"
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
	void initializefile();

public:
	HashBlockFile(std::string name, int bSize, RecordMethods* methods, int blockamount, bool createNew);
	virtual HashBlock* getCurrentBlock();
	int hashFunction(char* key);
	bool insertRecord(const char* key, const char* recordBytes, short size);
	bool internalInsertRecord(const char* key, const char* recordBytes,
			short size, bool force);
	//bool removeRecord(BlockFile* file, int blockNumber, Record* record);
	//void updateRecord(BlockFile* file, int blockNumber, Record* record);
	virtual void printContent();
	virtual void loadBlock(int blockNumber);
	virtual void saveBlock();
	virtual ~HashBlockFile();
};

#endif /* HASHBLOCKFILE_H_ */
