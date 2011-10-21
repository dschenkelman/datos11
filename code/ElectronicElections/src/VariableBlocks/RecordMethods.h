/*
 * RecordComparer.h
 *
 *  Created on: Sep 11, 2011
 *      Author: damian
 */

#ifndef RECORDMETHODS_H_
#define RECORDMETHODS_H_

#include "VariableRecord.h"

// used to compare records by primary key
class RecordMethods
{
public:
	// returns 0 if the keys are equal
	// > 0 => if key is greater than key in recordBytes
	// < 0 => if key in recordBytes is greater than key
	virtual int compare(const char* key, const char* recordBytes, int recordSize) = 0;
	virtual int compareKey(const char* key, const char* recordKey, int recordSize){return 0;}
	virtual void print(const char* recordBytes, int recordSize) = 0;
	virtual void printKey(const char* recordBytes, int recordSize) = 0;
	virtual VariableRecord* getKeyRecord(const char* recordBytes, int recordSize) = 0;
};

#endif /* RECORDMETHODS_H_ */
