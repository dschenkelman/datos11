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
#include <stack>

class TreeBlockFile: public BaseVariableBlockFile
{
	FreeBlockManager freeBlockManager;
	TreeBlock* currentBlock;
	TreeBlock* currentLeaf;
	bool isLeaf;
	std::stack<TreeBlock*> blockStack;
	std::stack<int> blockNumberStack;
public:
	TreeBlockFile(std::string& fileName, int bSize,
			RecordMethods *methods, bool createNew);
	FreeBlockManager& getFreeBlockManager();
	bool isCurrentLeaf();
	void pushBlock();
	void popBlock();
	TreeBlock* popAndKeep();
	void deleteKeptLeaf();
	virtual TreeBlock* getCurrentBlock();
	virtual void printContent();
	virtual void loadBlock(int blockNumber);
	virtual void saveBlock();
	void swapBlockKind();
	virtual ~TreeBlockFile();
};

#endif /* TREEBLOCKFILE_H_ */
