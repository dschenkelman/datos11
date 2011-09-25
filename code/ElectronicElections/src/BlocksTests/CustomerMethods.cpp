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
#include "./../Blocks/Constants.h"

using namespace std;

CustomerMethods::CustomerMethods()
{
}

Customer* CustomerMethods::getCustomerFromRecord(const char *recordBytes, int recordSize)
{
    int position = 0;
    Customer* c = new Customer;
    int fieldSize;
    char variableFieldSize;
    memcpy(&variableFieldSize, recordBytes + position, sizeof(char));
    c->firstName = new char[variableFieldSize];
    memcpy(c->firstName, recordBytes + position+1, variableFieldSize);
    position += variableFieldSize +1; //plus 1 for the fieldSize byte
    memcpy(&variableFieldSize, recordBytes + position, sizeof(char));
    c->lastName = new char[variableFieldSize];
    memcpy(c->lastName, recordBytes + position+1, variableFieldSize);
    position += variableFieldSize+1; //plus 1 for the fieldSize byte
    fieldSize = 4; //static balance, don't need to read it
    memcpy(&c->balance, recordBytes + position, fieldSize);
    position += fieldSize;

    return c;
}

int CustomerMethods::compare(const char* key, const char* recordBytes, int recordSize)
{
    Customer* c = this->getCustomerFromRecord(recordBytes, recordSize);

    int l1 = strlen(c->firstName) + 1;
    int l2 = strlen(c->lastName) + 1;
    char* recordKey = new char[l1 + l2 - 1];
    memset(recordKey, 0, l1 + l2 - 1);
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
