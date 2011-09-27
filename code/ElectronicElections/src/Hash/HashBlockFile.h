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

class HashBlockFile
{
	HashBlock* currentBlock;
	SimpleVariableBlockFile* overflowFile;
	std::string fileName;
	RecordMethods *recordMethods;
	std::fstream dataFile;
	long blockSize;
	SimpleVariableBlock *currentBlock;
	int loadedBlockNumber;
	int blockAmount;
public:
	HashBlockFile(string& name, int bSize, RecordMethods* methods, int blockamount, bool createNew);
	virtual HashBlock* getCurrentBlock();
	int hashFunction(char* key);
	bool insertRecord(char* key, VariableRecord* record);
	//bool removeRecord(BlockFile* file, int blockNumber, Record* record);
	//void updateRecord(BlockFile* file, int blockNumber, Record* record);
	virtual ~HashBlockFile();
};

#endif /* HASHBLOCKFILE_H_ */
