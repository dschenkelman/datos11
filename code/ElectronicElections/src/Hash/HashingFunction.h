/*
 * HashingFunction.h
 *
 *  Created on: Sep 20, 2011
 *      Author: gabriel
 */

#ifndef HASHINGFUNCTION_H_
#define HASHINGFUNCTION_H_

#include <stdlib.h>

class HashingFunction
{
	
public:
	
	HashingFunction();
	~HashingFunction();
	
	virtual int hashingFuntion(char* keyRecord, int count) =0;
	
	//int hashingDNI(char* keyRecord);
	//int hashingDistrict(char* keyRecord);
	//int hashingElection(char* keyRecord);
	
	// generic functions
	static long hashString(char* string);
};

#endif /* HASHINGFUNCTION_H_ */
	
