/*
 * HashingFunctions.h
 *
 *  Created on: Sep 20, 2011
 *      Author: gabriel
 */

#ifndef HASHINGFUNCTIONS_H_
#define HASHINGFUNCTIONS_H_

#include <stdlib.h>

class HashingFunctions
{
	char* fileName;
	int totalBlocks;
	
public:
	
	HashingFunctions(char* file, int blocksCount);
	~HashingFunctions(); 
	
	int hashing(char* keyRecord);
	
	int hashingDNI(char* keyRecord);
	int hashingDistrict(char* keyRecord);
	int hashingElection(char* keyRecord);
	
	// generic functions
	static long hashString(char* string);
};

#endif /* HASHINGFUNCTIONS_H_ */
	
