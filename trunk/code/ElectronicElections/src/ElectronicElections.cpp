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
#include "Voting.h"
#include <time.h>

using namespace std;

int run_tests()
{
	cout << "***** RUNNING TESTS *****" << endl;
	srand(time(NULL));
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
	while (1) {
		int action = Menu(login_type,2).ask();
		if (action == 0) { // VOTE
			cout << "Not available";
		} else if (action == 1) { // ADMIN
			string user = Menu::raw_input("User");
			string passwd = Menu::raw_input("Password");
			// TODO VERIFY LOGIN
			option *admin_action = new option[7];
			admin_action[0].label = "Mantener distritos";
			admin_action[1].label = "Mantener votantes";
			admin_action[2].label = "Mantener elecciones";
			admin_action[3].label = "Mantener cargos";
			admin_action[4].label = "Mantener listas";
			admin_action[5].label = "Mantener candidatos";
			admin_action[6].label = "Informar resultados";
			action = Menu(admin_action,7).ask();
			switch (action){
							case 0:
								break;
							case 1:
								break;
							case 2:
								break;
							case 3:
								break;
							case 4:
								break;
							case 5:
								break;
							case 6:
								break;
			}
		}
	}
	return 0;
}
