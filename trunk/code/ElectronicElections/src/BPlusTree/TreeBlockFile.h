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
#include "FreeBlockManager.h"

class TreeBlockFile: public BaseVariableBlockFile
{
	FreeBlockManager freeBlockManager;
	TreeBlock* currentBlock;
	bool isLeaf;
public:
	TreeBlockFile(std::string& fileName, int bSize,
			RecordMethods *methods, bool createNew);
	FreeBlockManager& getFreeBlockManager();
	bool isCurrentLeaf();
	virtual TreeBlock* getCurrentBlock();
	virtual void printContent();
	virtual void loadBlock(int blockNumber);
	virtual void saveBlock();
	virtual ~TreeBlockFile();
};

#endif /* TREEBLOCKFILE_H_ */
