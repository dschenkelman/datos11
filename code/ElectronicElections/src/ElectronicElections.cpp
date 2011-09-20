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

using namespace std;

int main()
{
	srand (time(NULL));
	BlockFileTests blockTests;
	blockTests.run();
	TreeBlockFileTests treeBlocktests;
	treeBlocktests.run();
	return 0;
}
