/*
 * CandidatesListMethods.h
 *
 *  Created on: 09/10/2011
 *      Author: alejandro
 */

#ifndef CANDIDATESLISTMETHODS_H_
#define CANDIDATESLISTMETHODS_H_

#include "../VariableBlocks/RecordMethods.h"

class CandidatesListMethods: public RecordMethods {
public:
	CandidatesListMethods();
	virtual int compare(const char* key, const char* recordBytes, int recordSize);
	virtual void print(const char* recordBytes, int recordSize);
	virtual void printKey(const char* key, int recordSize);
	virtual VariableRecord* getKeyRecord(const char* recordBytes, int recordSize);
	virtual ~CandidatesListMethods();
};

#endif /* CANDIDATESLISTMETHODS_H_ */
