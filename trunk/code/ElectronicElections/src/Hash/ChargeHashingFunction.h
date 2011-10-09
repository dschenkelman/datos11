/*
 * ChargeHashingFunction.h
 *
 *  Created on: 08/10/2011
 *      Author: gabriel
 */

#ifndef CHARGEHASHINGFUNCTION_H_
#define CHARGEHASHINGFUNCTION_H_

#include "HashingFunction.h"

class ChargeHashingFunction : HashingFunction
{

public:

	ChargeHashingFunction();
	~ChargeHashingFunction();

	virtual int hashingFunction(char* keyRecord, int count);
};

#endif /* CHARGEHASHINGFUNCTION_H_ */
