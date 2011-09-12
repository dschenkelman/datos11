/*
 * BlockFile.h
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#ifndef BLOCKFILE_H_
#define BLOCKFILE_H_

#include "Block.h"
#include "RecordComparer.h"
#include "Record.h"
#include <string.h>
#include <fstream>

class BlockFile
{
	std::string fileName;
    RecordComparer *recordComparer;
    std::fstream dataFile;
    long blockSize;
    Block *currentBlock;
    void positionAtBlock(int blockNumber);
public:
    BlockFile(std::string& fileName, int bSize, RecordComparer *comparer, bool createNew);
    bool insertRecord(char *key, char* recordBytes, int size);
    void loadBlock(int blockNumber);
    void saveBlock();
	virtual ~BlockFile();
};

#endif /* BLOCKFILE_H_ */
