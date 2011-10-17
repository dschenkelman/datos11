/*
 * ChargeHashingFunction.cpp
 *
 *  Created on: 08/10/2011
 *      Author: gabriel
 */

#include "ChargeHashingFunction.h"



ChargeHashingFunction::ChargeHashingFunction()
{
}

int ChargeHashingFunction::hashingFunction(const char* keyRecord, int blockCount)
{
	int len = strlen(keyRecord);
	int sum = 0;
	for(int i=1; i<len; i++){
		sum+= *(keyRecord+i);
	}
	return sum % blockCount;
}

ChargeHashingFunction::~ChargeHashingFunction()
{
}
