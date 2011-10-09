/*
 * LoadDataFiles.h
 *
 *  Created on: 09/10/2011
 *      Author: gabriel
 */

#ifndef LOADDATAFILES_H_
#define LOADDATAFILES_H_
#include <fstream>

class LoadDataFiles
{
	HashBlockFile* currentBlock;

	int totalBlocks;
	void initializefile();

public:
	LoadDataFiles();

	~LoadDataFiles();
};

#endif /* LOADDATAFILES_H_ */
