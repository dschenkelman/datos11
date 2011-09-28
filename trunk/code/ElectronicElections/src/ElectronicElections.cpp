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
#include "Hash/HashTest.h"

using namespace std;

int main()
{
	srand (time(NULL));
	/* Does not work yet
	TreeBlockFileTests treeBlocktests;
	treeBlocktests.run();
	*/
	//SimpleVariableBlockFileTests rlvTest;
	//rlvTest.run();
	cout << "HASH TESTS" << endl;
	HashTest hashTests;
	hashTests.run();
	cout << "LEAF NODE TESTS" << endl;
	LeafNodeTests leafTests;
	leafTests.run();
	return 0;
}
