/*
 * VoterHashingFunction.h
 *
 *  Created on: 09/10/2011
 *      Author: gabriel
 */

#ifndef VOTERHASHINGFUNCTION_H_
#define VOTERHASHINGFUNCTION_H_

#include "HashingFunction.h"
#include <cstring>

class VoterHashingFunction : public HashingFunction
{

public:

	VoterHashingFunction();
	virtual ~VoterHashingFunction();

	virtual int hashingFunction(const char* keyRecord, int count);
};
#endif /* VOTERHASHINGFUNCTION_H_ */
