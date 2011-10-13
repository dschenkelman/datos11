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
#include "../Entities/DistrictMethods.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "../BPlusTree/OverflowParameter.h"

LeafNodeTests::LeafNodeTests()
{
}

void LeafNodeTests::printResult(std::string testName, bool result)
{
	std::cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n"));
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
	printResult("testGetShouldGetRecordAndReturnTrue", testGetShouldGetRecordAndReturnTrue());
	printResult("testGetNonExistentRecordReturnsFalse", testGetNonExistentRecordReturnsFalse());
}

bool LeafNodeTests::testInsertDuplicatedRecordReturnsCorrectResult()
{
	CustomerMethods methods;
	SequenceTreeBlock block(64, &methods, false);
	LeafNode node(&block, &methods);

	VariableRecord recordOne;
	VariableRecord recordTwo;
	VariableRecord key;
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
	key.setBytes(recordKey, l1 + l2 - 1);

	OverflowParameter parameter;
	if(node.insert(&key, &recordOne, parameter) != Updated)
	{
		return false;
	}

	if(node.insert(&key, &recordTwo, parameter) != Duplicated)
	{
		return false;
	}

	return true;
}

bool LeafNodeTests::testInsertRecordInFullBlockReturnsOverflow()
{
	CustomerMethods methods;
    SequenceTreeBlock block(40, &methods, false);
    LeafNode node(&block, &methods);

    VariableRecord recordOne;
    VariableRecord recordTwo;
    VariableRecord keyRecord;
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
    keyRecord.setBytes(recordKey, l1 + l2 - 1);

    //std::string key = "keyTwo";
    VariableRecord keyTwo("keyTwo",strlen("keyTwo"));

	bool success = true;

	VariableRecord middleRecord;
	OverflowParameter parameter;
	if(node.insert(&keyRecord, &recordOne, parameter) != Updated)
	{
		success = false;
	}

	else if(node.insert(&keyTwo, &recordTwo, parameter) != Overflow)
	{
		success = false;
	}

	return success;
}

bool LeafNodeTests::testInsertLessThanFullSizeReturnsCorrectResult()
{
	VoterIndexMethods methods;
	SequenceTreeBlock block(64, &methods, false);
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
		memcpy(&key,&v.DNI,sizeof(int));
		VariableRecord keyRecord(key,sizeof(int));
		r.setBytes(keyRecord.getBytes(), 2*sizeof(int));
		OverflowParameter parameter;
		success = success && node.insert(&keyRecord, &r, parameter) == Updated;
	}

	node.print();

	return success;
}


bool LeafNodeTests::testInsertingWithOverflowPutsMiddleRecordInPassedParameter()
{
	CustomerMethods methods;
    SequenceTreeBlock block(32, &methods, false);
    LeafNode node(&block, &methods);

    VariableRecord recordOne;
    VariableRecord key;

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
    key.setBytes(recordKey, l1 + l2 - 1);

    VariableRecord recordTwo;
    VariableRecord custKey;
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
	custKey.setBytes(custRecordKey, sizeCust);

	OverflowParameter parameter;
	if(!(node.insert(&key, &recordOne, parameter) == Updated))
	{
		return false;
	}

	if(!(node.insert(&custKey, &recordTwo, parameter) == Overflow))
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
	SequenceTreeBlock block(64, &methods, false);
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
	SequenceTreeBlock block(32, &methods, false);
	LeafNode node(&block, &methods);

	VariableRecord recordOne;
	VariableRecord recordTwo;
	VariableRecord key;
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
	key.setBytes(recordKey, l1 + l2 - 1);

	OverflowParameter parameter;
	node.insert(&key, &recordOne, parameter);
	return node.update(recordKey, &recordTwo) == Overflow;
}

bool LeafNodeTests::testUpdateShouldUpdateRecordAndReturnUpdated()
{
	CustomerMethods methods;
	SequenceTreeBlock block(32, &methods, false);
	LeafNode node(&block, &methods);

	VariableRecord recordOne;
	VariableRecord recordTwo;
	VariableRecord key;
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
	key.setBytes(recordKey, l1 + l2 - 1);

	c.balance = -1;
	memcpy(recordBytes +2+ (l1+l2), &c.balance, sizeof(long));
	recordTwo.setBytes(recordBytes, size);

	OverflowParameter parameter;
	node.insert(&key, &recordOne, parameter);
	bool success = node.update(recordKey, &recordTwo) == Updated;

	node.print();

	return success;
}

bool LeafNodeTests::testDeleteReturnsNotFoundIfKeyIsNotPresent()
{
	CustomerMethods methods;
	SequenceTreeBlock block(32, &methods, false);
	LeafNode node(&block, &methods);

	return node.remove("NotValid") == NotFound;
}

bool LeafNodeTests::testDeleteReturnsUnderflowIfOccupiedSizeIsLessThanMinimum()
{
	CustomerMethods methods;
	SequenceTreeBlock block(32, &methods, false);
	LeafNode node(&block, &methods);

	VariableRecord recordOne;
	VariableRecord recordTwo;
	VariableRecord key;
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
	key.setBytes(recordKey,l1 + l2 - 1);
	OverflowParameter parameter;
	node.insert(&key, &recordOne, parameter);

	node.print();
	bool success = node.remove(recordKey) == Underflow;
	node.print();
	std::cout << std::endl;

	return success;
}

bool LeafNodeTests::testDeleteReturnsUpdatedIfOccupiedSizeIsMoreThanMinimumAndRecordWasDeleted()
{
	CustomerMethods methods;
	SequenceTreeBlock block(42, &methods, false);
	LeafNode node(&block, &methods);

	VariableRecord recordOne;
	VariableRecord key;
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
	key.setBytes(recordBytes, size);
	OverflowParameter parameter;
	node.insert(&key, &recordOne, parameter);

	VariableRecord recordTwo;
	VariableRecord custKey;
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
	custKey.setBytes(custRecordKey, sizeCust);

	bool success = true;
	success = success && node.insert(&custKey, &recordTwo, parameter) == Updated;
	node.print();
	success = success && node.remove(custRecordKey) == Updated;
	node.print();

	return success;
}

bool LeafNodeTests::testGetNonExistentRecordReturnsFalse()
{
	DistrictMethods districtMethods;
	District d("Tierra del Fuego");
	VariableRecord record;
	SequenceTreeBlock block(42, &districtMethods, false);
	LeafNode node(&block, &districtMethods);
	std::string districts[] = {	"Corrientes", "Entre Rios",
			"Chaco", "Chubut", "Cordoba",
			 "Santa Fe", "Santiago del Estero",
			"Mendoza", "Misiones", "Neuquen",
			"Tierra del Fuego", "Tucuman",
			};

	for (int i = 0; i < 4; i++)
	{
		insertDistrict(&node,districts[i]);
	}
	return !node.get(d.getKey(),&record, NULL);
}

bool LeafNodeTests::testGetShouldGetRecordAndReturnTrue()
{
	DistrictMethods districtMethods;
	District d("Entre Rios");
	VariableRecord record;
	SequenceTreeBlock block(80, &districtMethods, false);
	LeafNode node(&block, &districtMethods);
	std::string districts[] = {	"Corrientes", "Entre Rios",
			"Chaco", "Chubut", "Cordoba",
			 "Santa Fe", "Santiago del Estero",
			"Mendoza", "Misiones", "Neuquen",
			"Tierra del Fuego", "Tucuman",
			};

	for (int i = 0; i < 4; i++)
	{
		insertDistrict(&node,districts[i]);
	}
	bool result = node.get(d.getKey(),&record, NULL);
	bool recordInsertedIsTheSameThanReturned = districtMethods.compare(d.getBytes(),record.getBytes(),d.getSize()) == 0;

	return (recordInsertedIsTheSameThanReturned && (result));
}

void LeafNodeTests::insertDistrict(LeafNode* node, std::string key)
{
	District d(key);
	OverflowParameter parameter;
	VariableRecord dataRecord;
	VariableRecord keyRecord;
	dataRecord.setBytes(d.getBytes(), d.getSize());
	keyRecord.setBytes(d.getKey(), d.getKeySize());
	node->insert(&keyRecord, &dataRecord, parameter);
}

LeafNodeTests::~LeafNodeTests()
{
}
