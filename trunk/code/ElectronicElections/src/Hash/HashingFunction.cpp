/*
 * HashingFunction.cpp
 *
 *  Created on: Sep 20, 2011
 *      Author: gabriel
 */
 
#include "HashingFunction.h"
 
HashingFunction::HashingFunction()
{
}

HashingFunction::~HashingFunction()
{
}

/*
int HashingFunctions::hashingVotante(char* keyRecord)
{
	//keyRecord should be DNI number
	int dni = atoi(keyRecord); //cast to integer
	return ( dni % this->totalBlocks );
}
*/

long HashingFunction::hashString(char* string) {
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
