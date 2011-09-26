/*
 * BaseVariableBlockFile.h
 *
 *  Created on: Sep 25, 2011
 *      Author: damian
 */

#ifndef BASEVARIABLEBLOCKFILE_H_
#define BASEVARIABLEBLOCKFILE_H_

#include "BaseVariableBlock.h"
#include <fstream>

class BaseVariableBlockFile
{
protected:
	std::fstream dataFile;
	std::string fileName;
	int blockSize;
	int currentBlockNumber;
	RecordMethods* recordMethods;
public:
	BaseVariableBlockFile(std::string name, int bSize, RecordMethods* methods);
	virtual void printContent() = 0;
	virtual void loadBlock(int blockNumber) = 0;
	virtual BaseVariableBlock* getCurrentBlock() = 0;
	virtual void saveBlock() = 0;
	bool isAtEOF();
	void positionAtBlock(int blockNumber);
	virtual ~BaseVariableBlockFile();
};

#endif /* BASEVARIABLEBLOCKFILE_H_ */
