/*
 * ElectionsListMethods.h
 *
 *  Created on: 04/10/2011
 *      Author: gonzalo
 */

#ifndef ELECTIONSLISTMETHODS_H_
#define ELECTIONSLISTMETHODS_H_

#include "../VariableBlocks/RecordMethods.h"
#include "../VariableBlocks/Constants.h"

class ElectionsListMethods: public RecordMethods {
public:
	ElectionsListMethods();
	virtual int compare(const char* key, const char* recordBytes, int recordSize);
	virtual void print(const char* recordBytes, int recordSize);
	virtual void printKey(const char* key, int recordSize);
	virtual VariableRecord* getKeyRecord(const char* recordBytes, int recordSize);
	virtual ~ElectionsListMethods();
};

#endif /* ELECTIONSLISTMETHODS_H_ */
