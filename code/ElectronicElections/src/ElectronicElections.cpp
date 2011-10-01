//============================================================================
// Name        : ElectronicElections.cpp
// Author  : Datos11
// Version     :
// Copyright   : 
// Description : Trying block file out
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <string>
#include "BlocksTests/RLVBlockFileTests.h"
#include "BlocksTests/TreeBlockFileTests.h"
#include "BPlusTreeTests/LeafNodeTests.h"
#include "BPlusTreeTests/TreeTests.h"
#include "Hash/HashTest.h"
#include "EntitiesTests/DistrictTests.h"

using namespace std;

int main()
{
	srand (time(NULL));
	//SimpleVariableBlockFileTests rlvTest;
	//rlvTest.run();

	/* Does not work yet
	TreeBlockFileTests treeBlocktests;
	treeBlocktests.run();
	LeafNodeTests leafTests;
	leafTests.run();*/

//	cout << "HASH TESTS" << endl;
//	HashTest hashTests;
//	hashTests.run();
//	cout << "LEAF NODE TESTS" << endl;
//	LeafNodeTests leafTests;
//	leafTests.run();
//	cout << "DISTRICT TESTS" << endl;
//	DistrictTests districtTests;
//	districtTests.run();

	TreeTests treeTests;
	treeTests.run();
	return 0;
}
