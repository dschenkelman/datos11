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
#include "EntitiesTests/ChargeTests.h"
#include "Voting/Menu.h"

using namespace std;

int run_tests()
{
	cout << "***** RUNNING TESTS *****" << endl;
	srand (time(NULL));
	//SimpleVariableBlockFileTests rlvTest;
	//rlvTest.run();

	/* Does not work yet
	TreeBlockFileTests treeBlocktests;
	treeBlocktests.run();*/

	/*
	cout << "LeafNode Tests" << endl;
	LeafNodeTests leafTests;
	leafTests.run();

	cout << "District Tests" << endl;
	DistrictTests districtTests;
	districtTests.run();

	cout << "Hash Tests" << endl;
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
	at.run();

	cout << "Count Tests" << endl;
	CountTests ct;
	ct.run();

	cout << "Charge Tests" << endl;
	ChargeTests ct;
	ct.run();
	*/
	return 0;
}

int main() {
	option *main = new option[2];
	main[0].label = "Run main program";
	main[1].label = "Run tests";
	if ( Menu(main,2).ask() ) return run_tests();

	option *login_type = new option[2];
	login_type[0].label = "Vote";
	login_type[1].label = "Admin";
	int action = Menu(login_type,2).ask();
	if (action == 0) { // VOTE
		string dni = Menu::raw_input("DNI");
	} else if (action == 1) { // ADMIN
		string user = Menu::raw_input("User");
		string passwd = Menu::raw_input("Password");
	}
	return 0;
}
