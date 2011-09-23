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
#include "HashBlockFile.h"
//#include "../Blocks/RecordMethods.h"

using namespace std;

class Hash
{
	int blockAmount;
	HashBlockFile* file;
	Hash();

	public:

	Hash(HashBlockFile* file,int blockAmount);
	int hashFunction(char* key);

	//primitives. blockNumber comes from the hash function.
	bool insertRecord(char* key, Record* record);

	//bool removeRecord(BlockFile* file, int blockNumber, Record* record);
	//void updateRecord(BlockFile* file, int blockNumber, Record* record);

};

#endif /* HASH_H_ */
