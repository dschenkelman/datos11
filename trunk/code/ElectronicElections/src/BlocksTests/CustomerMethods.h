/*
 * CustomerComparer.h
 *
 *  Created on: Sep 12, 2011
 *      Author: damian
 */

#ifndef CUSTOMERMETHODS_H_
#define CUSTOMERMETHODS_H_

#include "../VariableBlocks/RecordMethods.h"
#include "Customer.h"
#include "../VariableBlocks/VariableRecord.h"

class CustomerMethods: public RecordMethods
{
private:
    Customer* getCustomerFromRecord(const char *recordBytes, int recordSize);
public:
    CustomerMethods();
    VariableRecord* getRecordFromCustomer(Customer* customer);
	virtual int compare(const char* key, const char* recordBytes, int recordSize);
	virtual void print(const char* recordBytes, int recordSize);
	virtual void printKey(const char* key, int recordSize);
	virtual ~CustomerMethods();
};

#endif /* CUSTOMERMETHODS_H_ */
