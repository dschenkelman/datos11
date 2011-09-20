/*
 * TreeBlockFile.h
 *
 *  Created on: Sep 20, 2011
 *      Author: damian
 */

#ifndef TREEBLOCKFILE_H_
#define TREEBLOCKFILE_H_

#include "BlockFile.h"
#include <string>
#include "TreeBlock.h"

class TreeBlockFile: public BlockFile
{
	TreeBlock* currentBlock;
public:
	TreeBlockFile(std::string& fileName, int bSize, int rSize, RecordMethods *methods, bool createNew);
	virtual TreeBlock* getCurrentBlock();
	virtual ~TreeBlockFile();
};

#endif /* TREEBLOCKFILE_H_ */
