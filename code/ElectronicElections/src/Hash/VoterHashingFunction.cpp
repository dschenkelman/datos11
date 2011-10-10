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

int VoterHashingFunction::hashingFunction(const char* keyRecord, int blockCount)
{
	// Modulize Dni
	int dni;
	memcpy(&dni, keyRecord, sizeof(int));
	return (dni % blockCount);
}

VoterHashingFunction::~VoterHashingFunction()
{
}
