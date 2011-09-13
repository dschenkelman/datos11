/*
 * CustomerComparer.h
 *
 *  Created on: Sep 12, 2011
 *      Author: damian
 */

#ifndef CUSTOMERCOMPARER_H_
#define CUSTOMERCOMPARER_H_

#include "Blocks/RecordComparer.h"
#include "Customer.h"

class CustomerComparer: public RecordComparer
{
private:
    Customer* getCustomerFromRecord(const char *recordBytes, int recordSize);
public:
	CustomerComparer();
	virtual int compare(const char* key, const char* recordBytes, int recordSize);
	virtual ~CustomerComparer();
};

#endif /* CUSTOMERCOMPARER_H_ */
