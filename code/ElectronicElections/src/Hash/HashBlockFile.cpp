/*
 * HashBlockFile.cpp
 *
 *  Created on: Sep 22, 2011
 *      Author: damian
 */

#include "HashBlockFile.h"

HashBlockFile::HashBlockFile(std::string& fileName, int bSize,
		int rSize, RecordMethods *methods, bool createNew, int blockAmount)
: BlockFile (fileName, bSize, rSize, methods, createNew)
{
	this->blockAmount = blockAmount;
	this->currentBlock = new HashBlock(bSize, rSize, methods);
	this->initializefile();
}

HashBlock* HashBlockFile::getCurrentBlock()
{
	return this->currentBlock;
}

HashBlockFile::~HashBlockFile()
{
	delete this->currentBlock;
}

void HashBlockFile::initializefile()
{
	char* block = new char [blockSize];
	memset(block,32,blockSize);
	for (int i= 0; i < blockAmount; i++)
	{
		this->dataFile.seekp(i*blockSize);
		this->dataFile.write(block,blockSize);
	}

}
