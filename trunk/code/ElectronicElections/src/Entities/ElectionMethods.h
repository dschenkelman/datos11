/*
 * ElectionMethods.h
 *
 *  Created on: 07/10/2011
 *      Author: lejosdelcielo
 */

#ifndef ELECTIONMETHODS_H_
#define ELECTIONMETHODS_H_

#include "../VariableBlocks/RecordMethods.h"

class ElectionMethods: public RecordMethods {
public:
	ElectionMethods();
	virtual int compare(const char* key, const char* recordBytes, int recordSize);
	virtual void print(const char* recordBytes, int recordSize);
	virtual void printKey(const char* key, int recordSize);
	virtual VariableRecord* getKeyRecord(const char* recordBytes, int recordSize);
	virtual ~ElectionMethods();
};

#endif /* ELECTIONMETHODS_H_ */
