/*
 * VoterHashingFunction.cpp
 *
 *  Created on: 09/10/2011
 *      Author: gabriel
 */

#include "VoterHashingFunction.h"

VoterHashingFunction::VoterHashingFunction()
{
}

int VoterHashingFunction::hashingFunction(char* keyRecord, int blockCount)
{
	// Modulize Dni
	int dni = atoi(keyRecord);
	return (dni % blockCount);
}

VoterHashingFunction::~VoterHashingFunction()
{
}
