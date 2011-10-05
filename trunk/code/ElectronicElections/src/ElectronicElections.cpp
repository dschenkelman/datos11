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
#include "EntitiesTests/ElectionsListTests.h"
#include "EntitiesTests/CandidateTests.h"
#include "EntitiesTests/AdministratorTests.h"
#include "EntitiesTests/CountTests.h"

using namespace std;

int main()
{
	srand (time(NULL));
	//SimpleVariableBlockFileTests rlvTest;
	//rlvTest.run();

	/* Does not work yet
	TreeBlockFileTests treeBlocktests;
	treeBlocktests.run();*/

//	cout << "LeafNode Tests" << endl;
//	LeafNodeTests leafTests;
//	leafTests.run();


//	cout << "District Tests" << endl;
//	DistrictTests districtTests;
//	districtTests.run();

/*	cout << "Hash Tests" << endl;
	HashTest hashTests;
	hashTests.run();

	cout << "Tree Tests" << endl;
	TreeTests treeTests;
	treeTests.run();

	cout << "Elections List Tests" << endl;
	ElectionsListTests electionTests;
	electionTests.run();

	cout << "Candidate Tests" << endl;
	CandidateTests ct;
	ct.run();

	cout << "Administrator Tests" << endl;
	AdministratorTests at;
	at.run();*/

	cout << "Count Tests" << endl;
	CountTests ct;
	ct.run();

	return 0;
}
