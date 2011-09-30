/*
 * DistrictMethods.h
 *
 *  Created on: 28/09/2011
 *      Author: alejandro
 */

#ifndef DISTRICTMETHODS_H_
#define DISTRICTMETHODS_H_

#include "../VariableBlocks/RecordMethods.h"
#include "../VariableBlocks/VariableRecord.h"
#include "../VariableBlocks/Constants.h"
#include "District.h"
#include <string>
using namespace std;

class DistrictMethods: public RecordMethods {
public:
	DistrictMethods();
	virtual ~DistrictMethods();
	virtual int compare(const char* key, const char* recordBytes, int recordSize) = 0;
	virtual void print(const char* recordBytes, int recordSize) = 0;
};

#endif /* DISTRICTMETHODS_H_ */
