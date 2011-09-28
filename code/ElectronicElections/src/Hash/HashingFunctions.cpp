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

long HashingFunctions::hashString(char* string) {
	long res = 0;
	long str_len = 0;
	while (1) {
		for (int i=0;i<16;i++) {
			if (!*(string+i)) {
				if (str_len > 16) return res;
				return res*16/str_len;
			}
			if (*(string+i) < 48 || *(string+i) > 122) res+=*(string+i)*i*64;
			res += (*(string+i)-48)*i*64;
			str_len++;
		}
	}
}
