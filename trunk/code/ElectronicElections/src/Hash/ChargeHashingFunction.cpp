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

int ChargeHashingFunction::hashingFunction(char* keyRecord, int blockCount)
{
	// Total blocks for charge are enough with 6 blocks but last 9
	// to be more intuitive, and have each charge in one block
	char letter = *keyRecord - 64;
	return (letter/(blockCount+1) + letter %blockCount);
}

ChargeHashingFunction::~ChargeHashingFunction()
{
}
