/*
 * LeafNodeTests.cpp
 *
 *  Created on: Sep 23, 2011
 *      Author: damian
 */

#include "LeafNodeTests.h"
#include "../BlocksTests/Customer.h"
#include "../BlocksTests/CustomerMethods.h"
#include "../BPlusTree/LeafNode.h"
#include "../BPlusTree/TreeBlock.h"
#include "../Voting/VoterIndexMethods.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

LeafNodeTests::LeafNodeTests()
{
}

void LeafNodeTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": Failed!!!\n"));
}

void LeafNodeTests::run()
{
	//printResult("testInsertLessThanFullSizeReturnsCorrectResult", testInsertLessThanFullSizeReturnsCorrectResult());
	printResult("testInsertDuplicatedRecordReturnsCorrectResult", testInsertDuplicatedRecordReturnsCorrectResult());
	printResult("testInsertRecordInFullBlockReturnsOverflow",testInsertRecordInFullBlockReturnsOverflow());
	printResult("testInsertarRegistroEnBloqueLlenoColocaRegistroDelMedioEnPuntero", testInsertarRegistroEnBloqueLlenoColocaRegistroDelMedioEnPuntero());
}

bool LeafNodeTests::testInsertDuplicatedRecordReturnsCorrectResult()
{
	CustomerMethods methods;
	SequenceTreeBlock block(64, &methods);
	LeafNode node(&block, &methods);

	VariableRecord recordOne;
	VariableRecord recordTwo;
	Customer c;
	c.firstName = "John";
	c.lastName = "Connor";
	c.balance = 100;
	int l1 = strlen(c.firstName) + 1;
	int l2 = strlen(c.lastName) + 1;
	// size = 16
	int size = l1+1 + l2+1 + sizeof (long); //sumo 2 bytes para cada tamaño del nombre y apellido
	char *recordKey = new char[l1 + l2 - 1];
	memset(recordKey, 0, l1 + l2 - 1);
	strcat(recordKey, c.firstName);
	strcat(recordKey, c.lastName);
	char *recordBytes = new char[size];
	memcpy(recordBytes, &l1, sizeof(char));
	memcpy(recordBytes + 1 , c.firstName, l1);
	memcpy(recordBytes +1+ l1, &l2, sizeof(char));
	memcpy(recordBytes +1+ l1 + 1, c.lastName, l2);

	memcpy(recordBytes +2+ (l1+l2), &c.balance, sizeof(long));

	recordOne.setBytes(recordBytes, size);
	recordTwo.setBytes(recordBytes, size);

	if(node.insert(recordKey, &recordOne) != Updated)
	{
		return false;
	}

	if(node.insert(recordKey, &recordTwo) != Duplicated)
	{
		return false;
	}

	return true;
}

bool LeafNodeTests::testInsertRecordInFullBlockReturnsOverflow()
{
	CustomerMethods methods;
    SequenceTreeBlock block(40, &methods);
    LeafNode node(&block, &methods);

    VariableRecord recordOne;
    VariableRecord recordTwo;
    Customer c;
    c.firstName = "John";
    c.lastName = "Connor";
    c.balance = 100;
    int l1 = strlen(c.firstName) + 1;
    int l2 = strlen(c.lastName) + 1;
    // size = 16
    int size = l1+1 + l2+1 + sizeof (long);
    char *recordKey = new char[l1 + l2 - 1];
    memset(recordKey, 0, l1 + l2 - 1);
    strcat(recordKey, c.firstName);
    strcat(recordKey, c.lastName);
    char *recordBytes = new char[size];
    memcpy(recordBytes, &l1, sizeof(char));
    memcpy(recordBytes + 1 , c.firstName, l1);
    memcpy(recordBytes +1+ l1, &l2, sizeof(char));
    memcpy(recordBytes +1+ l1 + 1, c.lastName, l2);

    memcpy(recordBytes +2+ (l1+l2), &c.balance, sizeof(long));

    recordOne.setBytes(recordBytes, size);
    recordTwo.setBytes(recordBytes, size);

    std::string key = "keyTwo";

	bool success = true;

	if(node.insert(recordKey, &recordOne) != Updated)
	{
		success = false;
	}

	else if(node.insert((char*) key.c_str(), &recordTwo) != Overflow)
	{
		success = false;
	}

	return success;
}

bool LeafNodeTests::testInsertLessThanFullSizeReturnsCorrectResult()
{
	VoterIndexMethods methods;
	SequenceTreeBlock block(64, &methods);
	LeafNode node(&block, &methods);

	bool success = true;
	for(long i = 0;i < 6;++i)
	{
		int n = rand() % 20000000 + 30000000;
		VoterIndex v;
		v.DNI = n;
		v.indexPointer = 0;
		VariableRecord r;
		char key[sizeof(int)];
		memcpy(&key, &v.DNI, sizeof(int));
		r.setBytes(key, 2*sizeof(int));
		success = success && node.insert(key, &r) == Updated;
	}

	node.print();

	return success;
}

// pasar a ingles!!
bool LeafNodeTests::testInsertarRegistroEnBloqueLlenoColocaRegistroDelMedioEnPuntero()
{
	CustomerMethods methods;
    SequenceTreeBlock block(70, &methods);
    LeafNode node(&block, &methods);

    VariableRecord recordOne;
    VariableRecord recordTwo;
    Customer c;
    c.firstName = "John";
    c.lastName = "Connor";
    c.balance = 100;
    int l1 = strlen(c.firstName) + 1;
    int l2 = strlen(c.lastName) + 1;
    // size = 16
    int size = l1+1 + l2+1 + sizeof (long);
    char *recordKey = new char[l1 + l2 - 1];
    memset(recordKey, 0, l1 + l2 - 1);
    strcat(recordKey, c.firstName);
    strcat(recordKey, c.lastName);
    char *recordBytes = new char[size];
    memcpy(recordBytes, &l1, sizeof(char));
    memcpy(recordBytes + 1 , c.firstName, l1);
    memcpy(recordBytes +1+ l1, &l2, sizeof(char));
    memcpy(recordBytes +1+ l1 + 1, c.lastName, l2);

    memcpy(recordBytes +2+ (l1+l2), &c.balance, sizeof(long));

    recordOne.setBytes(recordBytes, size);
    recordTwo.setBytes(recordBytes, size);

	VariableRecord recordThree;
	Customer cust;
	cust.firstName = "John";
	cust.lastName = "Riquelme";
	cust.balance = 102;
	int lCust1 = strlen(cust.firstName) + 1; // 5
	int lCust2 = strlen(cust.lastName) + 1; // 9
	int sizeCust = lCust1 + 1 + lCust2 + 1 + sizeof(long); // 5 + 1 + 9 + 1 + 4
	char* custRecordKey = new char[sizeCust];
	memset(custRecordKey, 0, lCust1 + lCust2 - 1);
    strcat(custRecordKey, cust.firstName);
    strcat(custRecordKey, cust.lastName);
	char *custRecordBytes = new char[sizeCust];
    memcpy(custRecordBytes, &lCust1, sizeof(char));
    memcpy(custRecordBytes + 1 , cust.firstName, lCust1);
    memcpy(custRecordBytes +1+ lCust1, &lCust2, sizeof(char));
    memcpy(custRecordBytes +1+ lCust1 + 1, cust.lastName, lCust2);
	memcpy(custRecordBytes +2+ (lCust1+lCust2), &cust.balance, sizeof(long));

	recordThree.setBytes(custRecordBytes, sizeCust);

	if(!node.insert(recordKey, &recordOne) == Updated)
	{
		return false;
	}

	std::string key = "keyTwo";

	if(!node.insert((char*)key.c_str(), &recordTwo) == Updated)
	{
		return false;
	}

	if(!node.insert(custRecordKey, &recordThree) == Overflow)
	{
		return false;
	}

	if(recordThree.getSize() != 22)
	{
		return false;
	}

	// falta verificar que los bytes los traiga bien!
	return true;
}

LeafNodeTests::~LeafNodeTests()
{
}
