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
#include <string.h>
#include <fstream>

class BlockFile
{
	// object used to compare records
	// the BlockFile instance deletes in the destructor
	RecordComparer* recordComparer;
	// file that holds the data
	std::fstream dataFile;
	// size of each block [bytes]
	long blockSize;
	// block being pointed to
	Block* currentBlock;
public:
	BlockFile(std::string fileName, int bSize, RecordComparer* comparer);
	// loads the current block from the file
	void loadBlock(int blockNumber);
	// saves the current block to the file
	void saveBlock();
	virtual ~BlockFile();
};

#endif /* BLOCKFILE_H_ */
