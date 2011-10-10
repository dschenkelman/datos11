/*
 * ChargeHashingFunction.h
 *
 *  Created on: 08/10/2011
 *      Author: gabriel
 */

#ifndef CHARGEHASHINGFUNCTION_H_
#define CHARGEHASHINGFUNCTION_H_

#include "HashingFunction.h"
#include <cstring>

class ChargeHashingFunction : public HashingFunction
{

public:

	ChargeHashingFunction();
	~ChargeHashingFunction();

	virtual int hashingFunction(const char* keyRecord, int count);
};

#endif /* CHARGEHASHINGFUNCTION_H_ */
