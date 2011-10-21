/*
 * FreeBlockManager.h
 *
 *  Created on: Sep 30, 2011
 *      Author: damian
 */

#ifndef FREEBLOCKMANAGER_H_
#define FREEBLOCKMANAGER_H_

#include <string>
#include <fstream>
#include <vector>

class FreeBlockManager
{
private:
	void loadFreeBlocks();
    void saveFreeBlocks();
	std::fstream file;
	std::vector<int> blocks;
	std::string fileName;
	int size;
public:
	FreeBlockManager(std::string fName, bool createNew);
	int getNextBlock();
	void addFreeBlock(int block);
	virtual ~FreeBlockManager();
};

#endif /* FREEBLOCKMANAGER_H_ */
