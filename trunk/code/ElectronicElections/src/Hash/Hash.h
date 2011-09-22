/*
 * Hash.h
 *
 *  Created on: Sep 20, 2011
 *      Author: juanma
 */

#ifndef HASH_H_
#define HASH_H_

#include <string.h>
#include <fstream>
#include <iostream>
#include "../Blocks/BlockFile.h"
#include "../Blocks/Block.h"
//#include "../Blocks/RecordMethods.h"

using namespace std;

class Hash
{
	long blockAmount;
	BlockFile* file;
	//Block* currentBlock;

	bool internalInsertRecord(int blockNumber, Record* record);
	Hash();

	public:

	Hash(BlockFile* file,long blockAmount);
	int hashFunction(long);

	//primitives. blockNumber comes from the hash function.
	bool insertRecord(long key, Record* record);

	//bool removeRecord(BlockFile* file, int blockNumber, Record* record);
	//void updateRecord(BlockFile* file, int blockNumber, Record* record);

};

#endif /* HASH_H_ */
