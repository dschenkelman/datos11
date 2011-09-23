/*
 * HashingFunctions.cpp
 *
 *  Created on: Sep 20, 2011
 *      Author: gabriel
 */
 
#include "HashingFunctions.h"
 
HashingFunctions::HashingFunctions(char* file, int blockCount)
{
	this->fileName = file;
	this->totalBlocks = blockCount;
}

HashingFunctions::~HashingFunctions()
{

}

int HashingFunctions::hashing(char* keyRecord)
{
	//if( strcmp(this->fileName, "Padron" ) )
		return hashingDNI(keyRecord);
		
	/*if( strcmp(this->fileName, "Distritos" ) )
		return hashingDistrict(keyRecord);
		
	if( strcmp(this->fileName, "Elecciones" ) )
		return hashingElection(keyRecord);
		*/
}

int HashingFunctions::hashingDNI(char* keyRecord)
{
	//keyRecord should be DNI number
	int dni = atoi(keyRecord); //cast to integer
	return ( dni % this->totalBlocks );
}

int HashingFunctions::hashingDistrict(char* keyRecord)
{
	// TODO: implement Distric hashing depending its key value.
	return 0;
}

int HashingFunctions::hashingElection(char* keyRecord)
{
	// TODO: implement Election hashing depending its key value.
	return 0;
}
