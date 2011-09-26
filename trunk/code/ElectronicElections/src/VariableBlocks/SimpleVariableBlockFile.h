/*
 * RLVBlockFile.h
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#ifndef RLVBLOCKFILE_H_
#define RLVBLOCKFILE_H_

#include "SimpleVariableBlock.h"
#include "BaseVariableBlockFile.h"
#include "./../Blocks/RecordMethods.h"
#include "VariableRecord.h"
#include <string.h>
#include <fstream>

class SimpleVariableBlockFile: public BaseVariableBlockFile
{
	std::string fileName;
    std::fstream dataFile;
    SimpleVariableBlock *currentBlock;
    int blockAmount;
    char* positionToDataBlocks;
    void positionAtBlock(int blockNumber);
    bool internalInsertRecord(const char* key,
    		const char* recordBytes, short size, bool force);
    bool isAtEOF();
    void updateBlockAmount();
public:
    SimpleVariableBlockFile(std::string& fileName, int bSize, RecordMethods *methods, bool createNew);
    void printContent();
    int getFirstFreeEmptyBlock();
    bool insertRecord(const char *key, const char* recordBytes, short size);
    bool updateRecord(const char *key, const char* recordBytes, short size);
    bool removeRecord(const char *key);
    bool getRecord(const char* key, VariableRecord** rec);
    virtual void loadBlock(int blockNumber);
    virtual SimpleVariableBlock* getCurrentBlock();
    void saveBlock();
	virtual ~SimpleVariableBlockFile();
};

#endif /* RLVBLOCKFILE_H_ */
