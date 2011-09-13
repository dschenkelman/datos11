/*
 * Customer.cpp
 *
 *  Created on: 12/09/2011
 *      Author: gabriel
 */

#include "Customer.h"

using namespace std;

Customer::Customer(char* firstName, char* lastName, customerComparer *comparer)
{
	strncpy(this->fName, firstName, strlen(firstName) );
	strncpy(this->lName, lastName, strlen(lastName) );
	//What comparer is for?
	//ToImplement
	this->customerComparer = comparer;
}

char* Customer::firstName()
{
	return this->fName;
}

char* Customer::lastName()
{
	return this->lName;
}

int Customer::compare(Customer c1, Customer c2)
{
	//Comparing by fName, lName or balance
	return this->customerComparer->compare(c1, c2); //To compare by 'comparingType'
}


