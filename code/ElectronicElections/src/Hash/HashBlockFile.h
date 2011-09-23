/*
 * HashBlockFile.h
 *
 *  Created on: Sep 22, 2011
 *      Author: damian
 */

#ifndef HASHBLOCKFILE_H_
#define HASHBLOCKFILE_H_

#include "../Blocks/BlockFile.h"
#include "HashBlock.h"

class HashBlockFile: public BlockFile
{
	HashBlock* currentBlock;
public:
	HashBlockFile(std::string& fileName, int bSize,
			int rSize, RecordMethods *methods, bool createNew);
	virtual HashBlock* getCurrentBlock();
	virtual ~HashBlockFile();
};

#endif /* HASHBLOCKFILE_H_ */
