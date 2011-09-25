/*
 * RLVBlockFile.h
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#ifndef RLVBLOCKFILE_H_
#define RLVBLOCKFILE_H_

#include "SimpleVariableBlock.h"
#include "./../Blocks/RecordMethods.h"
#include "VariableRecord.h"
#include <string.h>
#include <fstream>

class SimpleVariableBlockFile
{
	std::string fileName;
	RecordMethods *recordMethods;
    std::fstream dataFile;
    long blockSize;
    SimpleVariableBlock *currentBlock;
    int loadedBlockNumber;
    int blockAmount;
    char* positionToDataBlocks;
    void positionAtBlock(int blockNumber);
    bool internalInsertRecord(const char* key,
    		const char* recordBytes, int size, bool force);
    bool isAtEOF();
    void updateBlockAmount();

public:
    SimpleVariableBlockFile(std::string& fileName, int bSize, RecordMethods *methods, bool createNew);
    void printContent();
    int getFirstFreeEmptyBlock();
    bool insertRecord(const char *key, const char* recordBytes, int size);
    bool updateRecord(const char *key, const char* recordBytes, int size);
    bool removeRecord(const char *key);
    bool getRecord(const char* key, VariableRecord** rec);
    void loadBlock(int blockNumber);
    SimpleVariableBlock* getCurrentBlock();
    void saveBlock();
	virtual ~SimpleVariableBlockFile();
};

#endif /* RLVBLOCKFILE_H_ */
