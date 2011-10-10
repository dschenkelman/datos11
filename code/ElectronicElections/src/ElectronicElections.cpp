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
#include "Hash/HashBlockFile.h"
#include "Hash/DistrictHashingFunction.h"
#include "Hash/VoterHashingFunction.h"
#include "Entities/District.h"
#include "Entities/DistrictMethods.h"
#include "Entities/Voter.h"
#include "Entities/VoterMethods.h"
#include "VariableBlocks/VariableRecord.h"
#include "BPlusTree/Tree.h"
#include "Voting/Log.h"
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
	districtTests.run();*/

//	cout << "Hash Tests" << endl;
//	HashTest hashTests;
//	hashTests.run();

	cout << "Tree Tests" << endl;
	TreeTests treeTests;
	treeTests.run();
	/*
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
	ct.run();*/

	return 0;
}

int main()
{
	bool debug = true;
	if (debug)
	{
		run_tests();
		return 0;
	}
	Log log;
	log << "Iniciando sistema";
	option *main = new option[2];
	main[0].label = "Run main program";
	main[1].label = "Run tests";
	if ( Menu(main,2).ask() == 1 ) return run_tests();

	option *login_type = new option[3];
	login_type[0].label = "Vote";
	login_type[1].label = "Admin";
	login_type[2].label = "Quit";
	while (1) {
		int action = Menu(login_type,3).ask();
		switch (action) {
			case 0:
				cout << "Not available" << endl;
				break;
			case 2:
				return 0;
				break;
			case 1:
				string user = Menu::raw_input("User");
				string passwd = Menu::raw_input("Password");
				// TODO VERIFY LOGIN
				option *admin_action = new option[8];
				admin_action[0].label = "Mantener distritos";
				admin_action[1].label = "Mantener votantes";
				admin_action[2].label = "Mantener elecciones";
				admin_action[3].label = "Mantener cargos";
				admin_action[4].label = "Mantener listas";
				admin_action[5].label = "Mantener candidatos";
				admin_action[6].label = "Informar resultados";
				admin_action[7].label = "Generar archivos vacios";
				action = Menu(admin_action,8).ask();
				if (action == 0) {
					HashBlockFile *hash_district = new HashBlockFile("District.dat", 512, new DistrictMethods, new DistrictHashingFunction, 300, false);
					option *district_action = new option[3];
					district_action[0].label = "Agregar distrito";
					district_action[1].label = "Eliminar distrito";
					district_action[2].label = "Imprimir hash de distritos";
					action = Menu(district_action,3).ask();
					if (action==0) {
						District d = District (Menu::raw_input("Nombre del distrito"));
						hash_district->insertRecord(d.getKey(), new VariableRecord(d.getBytes(), d.getSize()) ) ? cout << "OK" << endl : cout << "FAILED" << endl;
					} else if (action==1) {
						District d = District (Menu::raw_input("Nombre del distrito"));
						hash_district->removeRecord(d.getKey()) ? cout << "OK" << endl : cout << "FAILED" << endl;
					} else if (action==2) hash_district->printContent();
				} else if (action == 1) {
					HashBlockFile *hash_voter = new HashBlockFile("Voter.dat", 1024*10, new VoterMethods, new VoterHashingFunction, 2800, false); // para 28 mil votantes
					option *voter_action = new option[5];
					voter_action[0].label = "Agregar votante";
					voter_action[1].label = "Cambio de domicilio";
					voter_action[2].label = "Cambio de distrito";
					voter_action[3].label = "Cambio de clave";
					voter_action[4].label = "Eliminar votante";
					action = Menu(voter_action,3).ask();
					if (action==0) {
						Voter v = Voter(atoi(Menu::raw_input("DNI").c_str()), Menu::raw_input("Nombre"), Menu::raw_input("Contraseña"), Menu::raw_input("Direccion"), Menu::raw_input("Distrito"), std::vector<ElectionKey>());
						hash_voter->insertRecord(v.getKey(), new VariableRecord(v.getBytes(), v.getSize()));
					} else if (action==1) {
						VariableRecord *record;
						Voter v = Voter(atoi(Menu::raw_input("DNI").c_str()), NULL, NULL, NULL, NULL, std::vector<ElectionKey>());
						hash_voter->getRecord(v.getKey(), &record);
						v.setBytes(record->getBytes());
						v.setAddress(Menu::raw_input("Nueva direccion"));
						record->setBytes(v.getBytes(), v.getSize());
						hash_voter->updateRecord(v.getKey(), record);
					}
				} else if (action == 7) {
					cout << "Generando archivo de distritos" << endl;
					HashBlockFile("District.dat", 512, NULL, NULL, 300, true);
					cout << "Generando archivo de votantes" << endl;
					HashBlockFile("Voter.dat", 1024*10, NULL, NULL, 2800, true); // para 28 mil votantes
					cout << "Generando archivo de elecciones" << endl;
					Tree("Election.dat", 512, NULL, true);
					cout << "Generando archivo de listas" << endl;
					Tree("CandidatesList.dat", 512, NULL, true);
					cout << "Generando archivo de conteo" << endl;
					Tree("Count.dat", 512, NULL, true);
					cout << "Generando archivo de cargos" << endl;
					HashBlockFile("Charge.dat", 512, NULL, NULL, 300, true);
					cout << "Generando archivo de candidato" << endl;
					Tree("Candidate.dat", 512, NULL, true);
				}
				break;
		}
	}
	return 0;
}
