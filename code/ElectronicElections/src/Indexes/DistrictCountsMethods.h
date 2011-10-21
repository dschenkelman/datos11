/*
 * DistrictCountsMethods.h
 *
 *  Created on: Oct 17, 2011
 *      Author: damian
 */

#ifndef DISTRICTCOUNTSMETHODS_H_
#define DISTRICTCOUNTSMETHODS_H_

#include "../VariableBlocks/RecordMethods.h"

class DistrictCountsMethods: public RecordMethods
{
public:
	DistrictCountsMethods();
	virtual int compare(const char* key, const char* recordBytes, int recordSize);
	virtual int compareKey(const char* key, const char* recordKey, int recordSize);
	virtual void print(const char* recordBytes, int recordSize);
	virtual void printKey(const char* recordBytes, int recordSize);
	virtual VariableRecord* getKeyRecord(const char* recordBytes, int recordSize);
	virtual ~DistrictCountsMethods();
};

#endif /* DISTRICTCOUNTSMETHODS_H_ */
