/*
 * ChargeMethods.h
 *
 *  Created on: 06/10/2011
 *      Author: lejosdelcielo
 */

#ifndef CHARGEMETHODS_H_
#define CHARGEMETHODS_H_

#include "../VariableBlocks/RecordMethods.h"

class ChargeMethods: public RecordMethods {
public:
	ChargeMethods();
	virtual int compare(const char* key, const char* recordBytes, int recordSize);
	virtual void print(const char* recordBytes, int recordSize);
	virtual void printKey(const char* key, int recordSize);
	virtual VariableRecord* getKeyRecord(const char* recordBytes, int recordSize);
	virtual ~ChargeMethods();
};

#endif /* CHARGEMETHODS_H_ */
