/*
 * BlockFile.h
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#ifndef BLOCKFILE_H_
#define BLOCKFILE_H_

#include <string.h>

class BlockFile
{
	// size of each block in bytes
	static const int DEFAULT_BLOCK_SIZE = 512;
public:
	BlockFile(std::string fileName, bool createNew);
	virtual ~BlockFile();
};

#endif /* BLOCKFILE_H_ */
