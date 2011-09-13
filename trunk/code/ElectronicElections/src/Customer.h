/*
 * Customer.h
 *
 *  Created on: 12/09/2011
 *      Author: gabriel
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include "CustomerComparer.h"

class Customer
{
	CustomerComparer *customerComparer;
    char* fName;
    char* lName;
    long balance;

public:
    Customer(char* firstName, char* lastName, CustomerComparer *comparer);
    char* firstName();
    char* lastName();
    int compare(Customer c1, Customer c2);
};

#endif /* CUSTOMER_H_ */
