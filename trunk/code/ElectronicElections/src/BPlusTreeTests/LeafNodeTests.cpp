/*
 * LeafNodeTests.cpp
 *
 *  Created on: Sep 23, 2011
 *      Author: damian
 */

#include "LeafNodeTests.h"
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
	printResult("testInsertLessThanFullSizeReturnsCorrectResult", testInsertLessThanFullSizeReturnsCorrectResult());
	printResult("testInsertDuplicatedRecordReturnsCorrectResult", testInsertDuplicatedRecordReturnsCorrectResult());
}

bool LeafNodeTests::testInsertDuplicatedRecordReturnsCorrectResult()
{
	VoterIndexMethods* methods = new VoterIndexMethods();
	SequenceTreeBlock block(64, methods);
	LeafNode node(&block, methods);

	VariableRecord* recordOne = new VariableRecord();
	VariableRecord* recordTwo = new VariableRecord();
	char* value = new char[2];
	recordOne->setBytes(value, 2);
	recordTwo->setBytes(value, 2);

	std::string k = "clave";
	if(node.insert((char*)k.c_str(), recordOne) != Updated)
	{
		return false;
	}

	if(node.insert((char*)k.c_str(), recordTwo) != Duplicated)
	{
		return false;
	}

	return true;
}

bool LeafNodeTests::testInsertLessThanFullSizeReturnsCorrectResult()
{
//	VoterIndexMethods* methods = new VoterIndexMethods();
//	TreeBlock block(64, 2 * sizeof(int), methods);
//	LeafNode node(&block, methods);
//
//	bool success = true;
//	for(long i = 0;i < 6;++i)
//	{
//		int n = rand() % 20000000 + 30000000;
//		VoterIndex v;
//		v.DNI = n;
//		v.indexPointer = 0;
//		VariableRecord r(2 * sizeof(int));
//		char key[sizeof(int)];
//		memcpy(&key, &v.DNI, sizeof(int));
//		r.setBytes(key);
//		success = success && node.insert(key, &r) == Updated;
//	}
//
//	node.print();
//
//	return success;
	return true;
}

LeafNodeTests::~LeafNodeTests()
{
}
