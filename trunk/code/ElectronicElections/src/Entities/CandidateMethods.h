/*
 * CandidateMethods.h
 *
 *  Created on: 04/10/2011
 *      Author: lejosdelcielo
 */

#ifndef CANDIDATEMETHODS_H_
#define CANDIDATEMETHODS_H_

#include "../VariableBlocks/RecordMethods.h"

class CandidateMethods: public RecordMethods {
public:
	CandidateMethods();
	virtual int compare(const char* key, const char* recordBytes, int recordSize);
	virtual int compareKey(const char* key, const char* recordKey, int recordSize);
	virtual void print(const char* recordBytes, int recordSize);
	virtual void printKey(const char* key, int recordSize);
	virtual VariableRecord* getKeyRecord(const char* recordBytes, int recordSize);
	virtual ~CandidateMethods();
};


#endif /* CANDIDATEMETHODS_H_ */
