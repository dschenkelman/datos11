/*
 * Customer.h
 *
 *  Created on: 12/09/2011
 *      Author: gabriel
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

class Customer
{
    char* fName;
    char* lName;
    long bal;

public:
    Customer(char* firstName, char* lastName);
    char* firstName();
    char* lastName();
    long balance();
};

#endif /* CUSTOMER_H_ */
