/*
 * RLVBlockFile.h
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#ifndef RLVBLOCKFILE_H_
#define RLVBLOCKFILE_H_

#include "RLVBlock.h"
#include "./../Blocks/RecordMethods.h"
#include "VariableRecord.h"
#include <string.h>
#include <fstream>

class RLVBlockFile
{
	std::string fileName;
	RecordMethods *recordMethods;
    std::fstream dataFile;
    long blockSize;
    RLVBlock *currentBlock;
    void positionAtBlock(int blockNumber);
    bool internalInsertRecord(const char* key,
    		const char* recordBytes, int size, bool force);
    bool isAtEOF();
public:
    RLVBlockFile(std::string& fileName, int bSize, RecordMethods *methods, bool createNew);
    void printContent();
    bool insertRecord(const char *key, const char* recordBytes, int size);
    bool updateRecord(const char *key, const char* recordBytes, int size);
    bool removeRecord(const char *key);
    bool getRecord(const char* key, VariableRecord** rec);
    void loadBlock(int blockNumber);
    RLVBlock* getCurrentBlock();
    void saveBlock();
	virtual ~RLVBlockFile();
};

#endif /* RLVBLOCKFILE_H_ */
