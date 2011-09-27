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
	std::cout << (testName.append(result ? ": \033[1;32mpassed\n\033[0m" : ": \033[1;31mFAILED!!!\n\033[0m"));
}

void LeafNodeTests::run()
{
	printResult("testInsertLessThanFullSizeReturnsCorrectResult", testInsertLessThanFullSizeReturnsCorrectResult());
	printResult("testInsertDuplicatedRecordReturnsCorrectResult", testInsertDuplicatedRecordReturnsCorrectResult());
	printResult("testInsertRecordInFullBlockReturnsOverflow",testInsertRecordInFullBlockReturnsOverflow());
	printResult("testInsertingWithOverflowPutsMiddleRecordInPassedParameter", testInsertingWithOverflowPutsMiddleRecordInPassedParameter());
	printResult("testUpdateNonExistentRecordReturnsNotFound", testUpdateNonExistentRecordReturnsNotFound());
	printResult("testUpdateShouldReturnOverflowIfRecordDoesNotFitNode", testUpdateShouldReturnOverflowIfRecordDoesNotFitNode());
	printResult("testUpdateShouldUpdateRecordAndReturnUpdated", testUpdateShouldUpdateRecordAndReturnUpdated());
	printResult("testDeleteReturnsNotFoundIfKeyIsNotPresent", testDeleteReturnsNotFoundIfKeyIsNotPresent());
	printResult("testDeleteReturnsUnderflowIfOccupiedSizeIsLessThanMinimum", testDeleteReturnsUnderflowIfOccupiedSizeIsLessThanMinimum());
	printResult("testDeleteReturnsUpdatedIfOccupiedSizeIsMoreThanMinimumAndRecordWasDeleted", testDeleteReturnsUpdatedIfOccupiedSizeIsMoreThanMinimumAndRecordWasDeleted());
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
	char recordKey[l1 + l2 - 1];
	memset(recordKey, 0, l1 + l2 - 1);
	strcat(recordKey, c.firstName);
	strcat(recordKey, c.lastName);
	char recordBytes[size];
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
	for(long i = 0;i < 5;++i)
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
bool LeafNodeTests::testInsertingWithOverflowPutsMiddleRecordInPassedParameter()
{
	CustomerMethods methods;
    SequenceTreeBlock block(32, &methods);
    LeafNode node(&block, &methods);

    VariableRecord recordOne;

    Customer c;
    c.firstName = "John";
    c.lastName = "Connor";
    c.balance = 100;
    int l1 = strlen(c.firstName) + 1;
    int l2 = strlen(c.lastName) + 1;
    // size = 16
    int size = l1+1 + l2+1 + sizeof (long);
    char recordKey[l1 + l2 - 1];
    memset(recordKey, 0, l1 + l2 - 1);
    strcat(recordKey, c.firstName);
    strcat(recordKey, c.lastName);
    char recordBytes[size];
    memcpy(recordBytes, &l1, sizeof(char));
    memcpy(recordBytes + 1 , c.firstName, l1);
    memcpy(recordBytes +1+ l1, &l2, sizeof(char));
    memcpy(recordBytes +1+ l1 + 1, c.lastName, l2);

    memcpy(recordBytes +2+ (l1+l2), &c.balance, sizeof(long));

    recordOne.setBytes(recordBytes, size);

    VariableRecord recordTwo;
	Customer cust;
	cust.firstName = "John";
	cust.lastName = "Riquelme";
	cust.balance = 102;
	int lCust1 = strlen(cust.firstName) + 1; // 5
	int lCust2 = strlen(cust.lastName) + 1; // 9
	int sizeCust = lCust1 + 1 + lCust2 + 1 + sizeof(long); // 5 + 1 + 9 + 1 + 4
	char custRecordKey[sizeCust];
	memset(custRecordKey, 0, lCust1 + lCust2 - 1);
    strcat(custRecordKey, cust.firstName);
    strcat(custRecordKey, cust.lastName);
	char custRecordBytes[sizeCust];
    memcpy(custRecordBytes, &lCust1, sizeof(char));
    memcpy(custRecordBytes + 1 , cust.firstName, lCust1);
    memcpy(custRecordBytes +1+ lCust1, &lCust2, sizeof(char));
    memcpy(custRecordBytes +1+ lCust1 + 1, cust.lastName, lCust2);
	memcpy(custRecordBytes +2+ (lCust1+lCust2), &cust.balance, sizeof(long));

	recordTwo.setBytes(custRecordBytes, sizeCust);

	if(!(node.insert(recordKey, &recordOne) == Updated))
	{
		return false;
	}

	if(!(node.insert(custRecordKey, &recordTwo) == Overflow))
	{
		return false;
	}

	if(recordTwo.getSize() != 18)
	{
		return false;
	}

	// falta verificar que los bytes los traiga bien!
	return true;
}

bool LeafNodeTests::testUpdateNonExistentRecordReturnsNotFound()
{
	VoterIndexMethods methods;
	SequenceTreeBlock block(64, &methods);
	LeafNode node(&block, &methods);

	int n = rand() % 20000000 + 30000000;
	VoterIndex v;
	v.DNI = n;
	v.indexPointer = 0;
	VariableRecord r;
	char key[sizeof(int)];
	memcpy(&key, &v.DNI, sizeof(int));
	r.setBytes(key, 2*sizeof(int));
	return node.update(key, &r) == NotFound;
}

bool LeafNodeTests::testUpdateShouldReturnOverflowIfRecordDoesNotFitNode()
{
	CustomerMethods methods;
	SequenceTreeBlock block(32, &methods);
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
	char recordKey[l1 + l2 - 1];
	memset(recordKey, 0, l1 + l2 - 1);
	strcat(recordKey, c.firstName);
	strcat(recordKey, c.lastName);
	char recordBytes[size];
	memcpy(recordBytes, &l1, sizeof(char));
	memcpy(recordBytes + 1 , c.firstName, l1);
	memcpy(recordBytes +1+ l1, &l2, sizeof(char));
	memcpy(recordBytes +1+ l1 + 1, c.lastName, l2);
	memcpy(recordBytes +2+ (l1+l2), &c.balance, sizeof(long));

	recordOne.setBytes(recordBytes, size);
	recordTwo.setBytes(recordBytes, 100);

	node.insert(recordKey, &recordOne);
	return node.update(recordKey, &recordTwo) == Overflow;
}

bool LeafNodeTests::testUpdateShouldUpdateRecordAndReturnUpdated()
{
	CustomerMethods methods;
	SequenceTreeBlock block(32, &methods);
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
	char recordKey[l1 + l2 - 1];
	memset(recordKey, 0, l1 + l2 - 1);
	strcat(recordKey, c.firstName);
	strcat(recordKey, c.lastName);
	char recordBytes[size];
	memcpy(recordBytes, &l1, sizeof(char));
	memcpy(recordBytes + 1 , c.firstName, l1);
	memcpy(recordBytes +1+ l1, &l2, sizeof(char));
	memcpy(recordBytes +1+ l1 + 1, c.lastName, l2);
	memcpy(recordBytes +2+ (l1+l2), &c.balance, sizeof(long));

	recordOne.setBytes(recordBytes, size);

	c.balance = -1;
	memcpy(recordBytes +2+ (l1+l2), &c.balance, sizeof(long));
	recordTwo.setBytes(recordBytes, size);

	node.insert(recordKey, &recordOne);
	bool success = node.update(recordKey, &recordTwo) == Updated;

	node.print();

	return success;
}

bool LeafNodeTests::testDeleteReturnsNotFoundIfKeyIsNotPresent()
{
	CustomerMethods methods;
	SequenceTreeBlock block(32, &methods);
	LeafNode node(&block, &methods);

	return node.remove("NotValid") == NotFound;
}

bool LeafNodeTests::testDeleteReturnsUnderflowIfOccupiedSizeIsLessThanMinimum()
{
	CustomerMethods methods;
	SequenceTreeBlock block(32, &methods);
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
	char recordKey[l1 + l2 - 1];
	memset(recordKey, 0, l1 + l2 - 1);
	strcat(recordKey, c.firstName);
	strcat(recordKey, c.lastName);
	char recordBytes[size];
	memcpy(recordBytes, &l1, sizeof(char));
	memcpy(recordBytes + 1 , c.firstName, l1);
	memcpy(recordBytes +1+ l1, &l2, sizeof(char));
	memcpy(recordBytes +1+ l1 + 1, c.lastName, l2);
	memcpy(recordBytes +2+ (l1+l2), &c.balance, sizeof(long));

	recordOne.setBytes(recordBytes, size);
	node.insert(recordKey, &recordOne);

	node.print();
	bool success = node.remove(recordKey) == Underflow;
	node.print();
	std::cout << std::endl;

	return success;
}

bool LeafNodeTests::testDeleteReturnsUpdatedIfOccupiedSizeIsMoreThanMinimumAndRecordWasDeleted()
{
	CustomerMethods methods;
	SequenceTreeBlock block(42, &methods);
	LeafNode node(&block, &methods);

	VariableRecord recordOne;
	Customer c;
	c.firstName = "John";
	c.lastName = "Connor";
	c.balance = 100;
	int l1 = strlen(c.firstName) + 1;
	int l2 = strlen(c.lastName) + 1;
	// size = 16
	int size = l1+1 + l2+1 + sizeof (long); //sumo 2 bytes para cada tamaño del nombre y apellido
	char recordKey[l1 + l2 - 1];
	memset(recordKey, 0, l1 + l2 - 1);
	strcat(recordKey, c.firstName);
	strcat(recordKey, c.lastName);
	char recordBytes[size];
	memcpy(recordBytes, &l1, sizeof(char));
	memcpy(recordBytes + 1 , c.firstName, l1);
	memcpy(recordBytes +1+ l1, &l2, sizeof(char));
	memcpy(recordBytes +1+ l1 + 1, c.lastName, l2);
	memcpy(recordBytes +2+ (l1+l2), &c.balance, sizeof(long));

	recordOne.setBytes(recordBytes, size);
	node.insert(recordKey, &recordOne);

	VariableRecord recordTwo;
	Customer cust;
	cust.firstName = "R";
	cust.lastName = "P";
	cust.balance = 102;
	int lCust1 = strlen(cust.firstName) + 1; // 2
	int lCust2 = strlen(cust.lastName) + 1; // 2
	int sizeCust = lCust1 + 1 + lCust2 + 1 + sizeof(long); // 2 + 1 + 2 + 1 + 4
	char custRecordKey[sizeCust];
	memset(custRecordKey, 0, lCust1 + lCust2 - 1);
	strcat(custRecordKey, cust.firstName);
	strcat(custRecordKey, cust.lastName);
	char custRecordBytes[sizeCust];
	memcpy(custRecordBytes, &lCust1, sizeof(char));
	memcpy(custRecordBytes + 1 , cust.firstName, lCust1);
	memcpy(custRecordBytes +1+ lCust1, &lCust2, sizeof(char));
	memcpy(custRecordBytes +1+ lCust1 + 1, cust.lastName, lCust2);
	memcpy(custRecordBytes +2+ (lCust1+lCust2), &cust.balance, sizeof(long));

	recordTwo.setBytes(custRecordBytes, sizeCust);

	bool success = true;
	success = success && node.insert(custRecordKey, &recordTwo) == Updated;
	node.print();
	success = success && node.remove(custRecordKey) == Updated;
	node.print();

	return success;
}

LeafNodeTests::~LeafNodeTests()
{
}
