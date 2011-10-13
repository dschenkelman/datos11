/*
 * DistrictHashingFunction.cpp
 *
 *  Created on: 08/10/2011
 *      Author: gabriel
 */

#include "DistrictHashingFunction.h"
#include <cstring>

DistrictHashingFunction::DistrictHashingFunction()
{
}

int DistrictHashingFunction::hashingFunction(const char* keyRecord, int blockCount)
{
	if(strlen(keyRecord) <= 4)
	{
		return 0; //districts with less or equal than 3 characters
	}
	char key[sizeof(int)];
	memcpy(key, keyRecord+1, sizeof(int));
	int numKey;
	memcpy(&numKey, key, sizeof(int));
	return numKey % blockCount;
}

DistrictHashingFunction::~DistrictHashingFunction()
{
}
