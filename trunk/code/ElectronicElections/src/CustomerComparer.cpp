/*
 * CustomerComparer.cpp
 *
 *  Created on: 12/09/2011
 *      Author: gabriel
 */

#include "CustomerComparer.h"

CustomerComparer::CustomerComparer(int compType)
{
	this->compareType = compType;
}

int CustomerComparer::compare(Customer c1, Customer c2)
{
	switch(this->compareType)
	{
		case FIRSTNAME_TYPE:
		{
			return ( strcmp(c1.firstName(), c2.firstName() ));
		}
		case LASTNAME_TYPE:
		{
			return ( strcmp(c1.lastName(), c2.lastName() ));
		}
		case BALANCE_TYPE:
		{
			if (c1.balance()< c2.balance() ) return -1;
			else if(c1.balance() == c2.balance() ) return 0; //Same balance
			else return 1;
		}
	}
}

