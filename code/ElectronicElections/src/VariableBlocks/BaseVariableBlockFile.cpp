/*
 * BaseVariableBlockFile.cpp
 *
 *  Created on: Sep 25, 2011
 *      Author: damian
 */

#include "BaseVariableBlockFile.h"

using namespace std;

BaseVariableBlockFile::BaseVariableBlockFile(string name, int bSize, RecordMethods* methods)
:fileName(name), blockSize(bSize), currentBlockNumber(0), recordMethods(methods)
{
}

void BaseVariableBlockFile::positionAtBlock(int blockNumber)
{
    long position = blockNumber * this->blockSize;
    this->dataFile.clear();
    this->dataFile.seekg(position, ios::beg);
    this->dataFile.seekp(position, ios::beg);
}

bool BaseVariableBlockFile::isAtEOF()
{
	long position = this->dataFile.tellg();
	this->dataFile.seekg(0, ios::end);
	long size = this->dataFile.tellg();
	this->dataFile.seekg(position, ios::beg);
	return position == size;
}

int BaseVariableBlockFile::getCurrentBlockNumber()
{
	return this->currentBlockNumber;
}

BaseVariableBlockFile::~BaseVariableBlockFile()
{
}
