/*
 * VoterHashingFunction.h
 *
 *  Created on: 09/10/2011
 *      Author: gabriel
 */

#ifndef VOTERHASHINGFUNCTION_H_
#define VOTERHASHINGFUNCTION_H_

#include "HashingFunction.h"

class VoterHashingFunction : HashingFunction
{

public:

	VoterHashingFunction();
	~VoterHashingFunction();

	virtual int hashingFunction(char* keyRecord, int count);
};
#endif /* VOTERHASHINGFUNCTION_H_ */
