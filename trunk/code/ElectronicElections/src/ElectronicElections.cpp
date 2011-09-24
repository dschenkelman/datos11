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
#include "BlocksTests/BlockFileTests.h"
#include "BlocksTests/TreeBlockFileTests.h"
#include "BPlusTreeTests/LeafNodeTests.h"
#include "Hash/HashTest.h"

using namespace std;

int main()
{
	srand (time(NULL));
	TreeBlockFileTests treeBlocktests;
	treeBlocktests.run();
	LeafNodeTests leafTests;
	leafTests.run();
	HashTest hash;
	hash.run();

	return 0;
}
