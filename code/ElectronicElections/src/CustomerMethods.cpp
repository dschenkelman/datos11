/*
 * CustomerComparer.cpp
 *
 *  Created on: Sep 12, 2011
 *      Author: damian
 */

#include "CustomerMethods.h"
#include "Customer.h"
#include <string.h>
#include <iostream>

using namespace std;

CustomerMethods::CustomerMethods()
{
}

Customer* CustomerMethods::getCustomerFromRecord(const char *recordBytes, int recordSize)
{
    int position = 0;
    Customer* c = new Customer;
    int fieldSize;
    memcpy(&fieldSize, recordBytes + position, 4);
    position += 4;
    c->firstName = new char[fieldSize];
    memcpy(c->firstName, recordBytes + position, fieldSize);
    position += fieldSize;
    memcpy(&fieldSize, recordBytes + position, 4);
    position += 4;
    c->lastName = new char[fieldSize];
    memcpy(c->lastName, recordBytes + position, fieldSize);
    position += fieldSize;
    memcpy(&fieldSize, recordBytes + position, 4);
    position += 4;
    memcpy(&c->balance, recordBytes + position, fieldSize);
    position += fieldSize;

    return c;
}

int CustomerMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
    Customer* c = this->getCustomerFromRecord(recordBytes, recordSize);

    int l1 = strlen(c->firstName);
    int l2 = strlen(c->lastName);
    char* recordKey = new char[l1 + l2 - 1];
    strcat(recordKey, c->firstName);
    strcat(recordKey, c->lastName);
    int cmpResult = strcmp(key, recordKey);

    delete[] recordKey;
    delete[] c->firstName;
    delete[] c->lastName;
    delete c;

    return cmpResult;
}

void CustomerMethods::print(const char *recordBytes, int recordSize)
{
	Customer* c = this->getCustomerFromRecord(recordBytes, recordSize);

	cout << c->firstName << " ";
	cout << c->lastName << " ";
	cout << c->balance << endl;

	delete[] c->firstName;
	delete[] c->lastName;
	delete c;
}

CustomerMethods::~CustomerMethods()
{
}
