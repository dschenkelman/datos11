/*
 * ByteOperatorsTests.cpp
 *
 *  Created on: Sep 18, 2011
 *      Author: damian
 */

#include "ByteOperators.h"
#include "ByteOperatorsTests.h"
#include <iostream>

using namespace std;

ByteOperatorsTests::ByteOperatorsTests()
{
}

void ByteOperatorsTests::printByte(char myByte)
{
     cout<<ByteOperators::isBitOne(myByte,0);
     cout<<ByteOperators::isBitOne(myByte,1);
     cout<<ByteOperators::isBitOne(myByte,2);
     cout<<ByteOperators::isBitOne(myByte,3);
     cout<<ByteOperators::isBitOne(myByte,4);
     cout<<ByteOperators::isBitOne(myByte,5);
     cout<<ByteOperators::isBitOne(myByte,6);
     cout<<ByteOperators::isBitOne(myByte,7);
     cout<<"\n";
}

void ByteOperatorsTests::run()
{
	cout<<"Start ByteOperator Testing...\n";
    //Test isBitOne
	char byte = 9;
	cout<<"Expected: 00001001\nCurrent:";
	printByte(byte);
	cout<<"\n";

	//Test setBit
	byte = 0;
	ByteOperators::setBit(&byte,0,1);
	ByteOperators::setBit(&byte,1,1);
	ByteOperators::setBit(&byte,5,1);
	ByteOperators::setBit(&byte,7,1);
	cout<<"Expected: 10100011\nCurrent:";
	printByte(byte);

	ByteOperators::setBit(&byte,1,0);
	ByteOperators::setBit(&byte,5,0);
	cout<<"Expected: 10000001\nCurrent:";
	printByte(byte);
}

ByteOperatorsTests::~ByteOperatorsTests()
{
}
