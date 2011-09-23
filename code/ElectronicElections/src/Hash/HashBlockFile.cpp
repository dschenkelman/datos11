/*
 * HashBlockFile.cpp
 *
 *  Created on: Sep 22, 2011
 *      Author: damian
 */

#include "HashBlockFile.h"

HashBlockFile::HashBlockFile(std::string& fileName, int bSize,
		int rSize, RecordMethods *methods, bool createNew)
: BlockFile (fileName, bSize, rSize, methods, createNew)
{
	this->currentBlock = new HashBlock(bSize, rSize, methods);
}


HashBlock* HashBlockFile::getCurrentBlock()
{
	return this->currentBlock;
}

HashBlockFile::~HashBlockFile()
{
	delete this->currentBlock;
}
