/*
 * BlockFile.h
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#ifndef BLOCKFILE_H_
#define BLOCKFILE_H_

#include "Block.h"
#include "RecordMethods.h"
#include "Record.h"
#include <string.h>
#include <fstream>

class BlockFile
{
	std::string fileName;
	RecordMethods *recordMethods;
    std::fstream dataFile;
    long blockSize;
    Block *currentBlock;
    void positionAtBlock(int blockNumber);
    bool isAtEOF();
public:
    BlockFile(std::string& fileName, int bSize, RecordMethods *methods, bool createNew);
    void printContent();
    bool insertRecord(const char *key, const char* recordBytes, int size);
    bool updateRecord(const char *key, const char* recordBytes, int size);
    bool removeRecord(const char *key);
    void loadBlock(int blockNumber);
    void saveBlock();
	virtual ~BlockFile();
};

#endif /* BLOCKFILE_H_ */
