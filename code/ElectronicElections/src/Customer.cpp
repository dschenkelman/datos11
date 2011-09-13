/*
 * Customer.cpp
 *
 *  Created on: 12/09/2011
 *      Author: gabriel
 */

#include "Customer.h"

using namespace std;

Customer::Customer(char* firstName, char* lastName, long balance)
{
	strncpy(this->fName, firstName, strlen(firstName) );
	strncpy(this->lName, lastName, strlen(lastName) );
	this->bal = balance;
}

char* Customer::firstName()
{
	return this->fName;
}

char* Customer::lastName()
{
	return this->lName;
}

long Customer::balance()
{
	return this->bal;
}


