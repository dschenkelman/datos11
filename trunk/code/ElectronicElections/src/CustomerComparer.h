/*
 * CustomerComparer.h
 *
 *  Created on: 12/09/2011
 *      Author: gabriel
 */

#ifndef CUSTOMERCOMPARER_H_
#define CUSTOMERCOMPARER_H_

#include "Customer.h"
#define FIRSTNAME_TYPE 1
#define LASTNAME_TYPE 2
#define BALANCE_TYPE 3

class CustomerComparer
{
	int compareType;

public:
    CustomerComparer(int compType);
    int compare(Customer c1, Customer c2);
    void comparingBy(int type); //set
};

#endif /* CUSTOMERCOMPARER_H_ */
