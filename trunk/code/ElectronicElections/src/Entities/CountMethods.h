/*
 * CountMethods.h
 *
 *  Created on: 05/10/2011
 *      Author: lejosdelcielo
 */

#ifndef COUNTMETHODS_H_
#define COUNTMETHODS_H_

#include "../VariableBlocks/RecordMethods.h"

class CountMethods: public RecordMethods {
public:
	CountMethods();
	virtual int compare(const char* key, const char* recordBytes, int recordSize);
	virtual void print(const char* recordBytes, int recordSize);
	virtual void printKey(const char* key, int recordSize);
	virtual VariableRecord* getKeyRecord(const char* recordBytes, int recordSize);
	virtual ~CountMethods();
};

#endif /* COUNTMETHODS_H_ */
