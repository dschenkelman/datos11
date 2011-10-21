/*
 * VoterMethods.h
 *
 *  Created on: 09/10/2011
 *      Author: lejosdelcielo
 */

#ifndef VOTERMETHODS_H_
#define VOTERMETHODS_H_

#include "../VariableBlocks/RecordMethods.h"

class VoterMethods: public RecordMethods {
public:
	VoterMethods();
	virtual int compare(const char* key, const char* recordBytes, int recordSize);
	virtual int compareKey(const char* key, const char* recordKey, int recordSize);
	virtual void print(const char* recordBytes, int recordSize);
	virtual void printKey(const char* key, int recordSize);
	virtual VariableRecord* getKeyRecord(const char* recordBytes, int recordSize);
	virtual ~VoterMethods();
};

#endif /* VOTERMETHODS_H_ */
