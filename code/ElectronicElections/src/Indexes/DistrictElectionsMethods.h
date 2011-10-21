/*
 * DistrictElectionMethods.h
 *
 *  Created on: Oct 16, 2011
 *      Author: damian
 */

#ifndef DISTRICTELECTIONSMETHODS_H_
#define DISTRICTELECTIONSMETHODS_H_

#include "../VariableBlocks/RecordMethods.h"

class DistrictElectionsMethods: public RecordMethods {
public:
	DistrictElectionsMethods();
	virtual int compare(const char* key, const char* recordBytes, int recordSize);
	virtual int compareKey(const char* key, const char* recordKey, int recordSize);
	virtual void print(const char* recordBytes, int recordSize);
	virtual void printKey(const char* recordBytes, int recordSize);
	virtual VariableRecord* getKeyRecord(const char* recordBytes, int recordSize);
	virtual ~DistrictElectionsMethods();
};

#endif /* DISTRICTELECTIONSMETHODS_H_ */
