/*
 * DistrictHashingFunction.h
 *
 *  Created on: 08/10/2011
 *      Author: gabriel
 */

#ifndef DISTRICT_HASHINGFUNCTION_H_
#define DISTRICT_HASHINGFUNCTION_H_

#include "HashingFunction.h"

class DistrictHashingFunction : HashingFunction
{

public:

	DistrictHashingFunction();
	~DistrictHashingFunction();

	virtual int hashingFunction(char* keyRecord, int count);
};

#endif /* DISTRICT_HASHINGFUNCTION_H_ */
