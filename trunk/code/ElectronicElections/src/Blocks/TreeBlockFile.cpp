/*
 * TreeBlockFile.cpp
 *
 *  Created on: Sep 20, 2011
 *      Author: damian
 */

#include "TreeBlockFile.h"

TreeBlockFile::TreeBlockFile(std::string& fileName, int bSize,
		int rSize, RecordMethods *methods, bool createNew) :
		BlockFile(fileName, bSize, rSize, methods, createNew)
{
	this->currentBlock = new TreeBlock(bSize, rSize, methods);
}

TreeBlock* TreeBlockFile::getCurrentBlock()
{
	return this->currentBlock;
}

TreeBlockFile::~TreeBlockFile()
{
	delete this->currentBlock;
}
