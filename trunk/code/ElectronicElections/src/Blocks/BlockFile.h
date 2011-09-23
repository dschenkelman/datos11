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
    int currentBlockNumber;
    void positionAtBlock(int blockNumber);
    bool isAtEOF();
protected:
    std::fstream dataFile;
    long blockSize;
public:
    BlockFile(std::string& fileName, int bSize, int rSize, RecordMethods *methods, bool createNew);
    void printContent();
    virtual Block* getCurrentBlock() = 0;
    void loadBlock(int blockNumber);
    void saveBlock();
	virtual ~BlockFile();
};

#endif /* BLOCKFILE_H_ */
