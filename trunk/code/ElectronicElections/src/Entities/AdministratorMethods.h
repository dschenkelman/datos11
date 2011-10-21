/*
 * AdministratorMethods.h
 *
 *  Created on: 04/10/2011
 *      Author: lejosdelcielo
 */

#ifndef ADMINISTRATORMETHODS_H_
#define ADMINISTRATORMETHODS_H_

#include "../VariableBlocks/RecordMethods.h"

class AdministratorMethods: public RecordMethods {
public:
	AdministratorMethods();
	virtual int compare(const char* key, const char* recordBytes, int recordSize);
	virtual int compareKey(const char* key, const char* recordKey, int recordSize);
	virtual void print(const char* recordBytes, int recordSize);
	virtual void printKey(const char* key, int recordSize);
	virtual VariableRecord* getKeyRecord(const char* recordBytes, int recordSize);
	virtual ~AdministratorMethods();
};

#endif /* ADMINISTRATORMETHODS_H_ */
