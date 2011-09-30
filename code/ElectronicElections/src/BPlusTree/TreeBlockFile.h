/*
 * TreeBlockFile.h
 *
 *  Created on: Sep 20, 2011
 *      Author: damian
 */

#ifndef TREEBLOCKFILE_H_
#define TREEBLOCKFILE_H_

#include "../VariableBlocks/BaseVariableBlockFile.h"
#include <string>
#include "TreeBlock.h"

class TreeBlockFile: public BaseVariableBlockFile
{
	TreeBlock* currentBlock;
	bool isLeaf;
public:
	TreeBlockFile(std::string& fileName, int bSize,
			RecordMethods *methods, bool createNew);
	bool isCurrentLeaf();
	virtual TreeBlock* getCurrentBlock();
	virtual void printContent();
	virtual void loadBlock(int blockNumber);
	virtual void saveBlock();
	virtual ~TreeBlockFile();
};

#endif /* TREEBLOCKFILE_H_ */
