/*
 * DistrictElectionMethods.h
 *
 *  Created on: Oct 16, 2011
 *      Author: damian
 */

#ifndef DISTRICTELECTIONMETHODS_H_
#define DISTRICTELECTIONMETHODS_H_

#include "../VariableBlocks/RecordMethods.h"

class DistrictElectionMethods: public RecordMethods {
public:
	DistrictElectionMethods();
	virtual int compare(const char* key, const char* recordBytes, int recordSize);
	virtual void print(const char* recordBytes, int recordSize);
	virtual void printKey(const char* recordBytes, int recordSize);
	virtual VariableRecord* getKeyRecord(const char* recordBytes, int recordSize);
	virtual ~DistrictElectionMethods();
};

#endif /* DISTRICTELECTIONMETHODS_H_ */
