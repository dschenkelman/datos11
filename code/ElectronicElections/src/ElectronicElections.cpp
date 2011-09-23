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
	/*HashBlockFileTests blockTests;
	blockTests.run();
	TreeBlockFileTests treeBlocktests;
	treeBlocktests.run();

	LeafNodeTests leafTests;
	leafTests.run();
	char* bytes = new char[50];
	memset(bytes,32,20);
	char* addThisbytes = new char[10];
	strcpy(addThisbytes,"HelloWorld");
	memcpy(bytes + 8, addThisbytes, 10);
	cout << bytes;
*/
	HashTest hash;
	hash.run();

	return 0;
}
