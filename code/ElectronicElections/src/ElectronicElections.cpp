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
#include "Voting/Voting.h"
#include "Hash/HashBlockFile.h"
#include "Hash/DistrictHashingFunction.h"
#include "Hash/VoterHashingFunction.h"
#include "Hash/ChargeHashingFunction.h"
#include "Entities/District.h"
#include "Entities/DistrictMethods.h"
#include "Entities/Voter.h"
#include "Entities/VoterMethods.h"
#include "Entities/Administrator.h"
#include "Entities/AdministratorMethods.h"
#include "Entities/Election.h"
#include "Entities/ElectionMethods.h"
#include "Entities/Charge.h"
#include "Entities/ChargeMethods.h"
#include "Entities/ElectionsList.h"
#include "Entities/ElectionsListMethods.h"
#include "Entities/Candidate.h"
#include "Entities/CandidateMethods.h"
#include "VariableBlocks/VariableRecord.h"
#include "BPlusTree/Tree.h"
#include "Voting/Log.h"
#include <time.h>
#include <sstream>

#include "Voting/LoadDataFiles.h"

using namespace std;

int run_tests()
{
	//LoadDataFiles dataFiles("config.txt");
	//dataFiles.readConfigFile();

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
	leafTests.run();*/

//	cout << "District Tests" << endl;
//	DistrictTests districtTests;
//	districtTests.run();

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
	CountTests countTests;
	countTests.run();

	cout << "Charge Tests" << endl;
	ChargeTests chargeTests;
	chargeTests.run();*/

	return 0;
}

int main() // Las pruebas se pueden correr con la opcion 1 muy facilmente, incluso desde eclipse
{			// No impedir el uso normal del sistema
	LoadDataFiles dataFiles("config.txt");
	Log log;
	log.write("Iniciando sistema", true, true);
	option main[2];
	main[0].label = "Run main program";
	main[1].label = "Run tests";
	if ( Menu(main,2).ask() == 1 ) return run_tests();

	option login_type[3];
	login_type[0].label = "Vote";
	login_type[1].label = "Admin";
	login_type[2].label = "Quit";
	while (1)
	{
		int action = Menu(login_type,3).ask();
		switch (action)
		{
			case 0:
				{
					bool voting = true;
					LoadDataFiles ldf("config.txt");
					ldf.readConfigFile();
					while(voting)
					{
						Voting vot(&ldf);

						 if(!vot.login())
						 {
							 cout << "Fallo login. Intente nuevamente" << endl;
						 }
						 else if(!vot.vote())
						 {
						 cout << "Fallo votacion. Intente nuevamente" << endl;
						 }
						 else voting = false;
						 break;
					}
				}
				break;
			case 2:
				return 0;
				break;
			case 1:
				string user = Menu::raw_input("User");
				string passwd = Menu::raw_input("Password");
				Tree *admin_tree;
				if(!dataFiles.canOpenAdminFile())
					admin_tree = dataFiles.createAdminFile();
				else
					admin_tree = dataFiles.getAdminFile();
				Administrator admin = Administrator(user, passwd);
				VariableRecord *adminrecord = new VariableRecord;
				admin_tree->get(admin.getKey(), adminrecord);
				delete adminrecord;
				// TODO - COMO OBTENGO EL OBJETO ADMINISTRADOR AHORA?
				while (1) {
					option admin_action[9];
					admin_action[0].label = "Mantener distritos";
					admin_action[1].label = "Mantener votantes";
					admin_action[2].label = "Mantener elecciones";
					admin_action[3].label = "Mantener cargos";
					admin_action[4].label = "Mantener listas";
					admin_action[5].label = "Mantener candidatos";
					admin_action[6].label = "Informar resultados";
					admin_action[7].label = "Generar archivos vacios";
					admin_action[8].label = "Volver";
					action = Menu(admin_action,9).ask();
					if (action == 0) {
						Tree district_tree ("District.dat", 512, &DistrictMethods(), false);
						option district_action[3];
						district_action[0].label = "Agregar distrito";
						district_action[1].label = "Eliminar distrito";
						district_action[2].label = "Imprimir arbol de distritos";
						action = Menu(district_action,3).ask();
						if (action==0) {
							District d = District (Menu::raw_input("Nombre del distrito"));
							VariableRecord record (d.getBytes(), d.getSize());
							int res = district_tree.insert(&record, &record);
							log.write(string("Agregando distrito ").append(d.getName()), res!=5, true);
						} else if (action==1) {
							District d = District (Menu::raw_input("Nombre del distrito"));
							int res = district_tree.remove(d.getKey());
							log.write(string("Eliminando distrito ").append(d.getName()), res!=4, true);
						} else if (action==2) district_tree.print();
					} else if (action == 1) {
						HashBlockFile hash_voter ("Voter.dat", 1024*10, &VoterMethods(), &VoterHashingFunction(), 2800, false); // para 28 mil votantes
						option voter_action[5];
						voter_action[0].label = "Agregar votante";
						voter_action[1].label = "Cambio de domicilio";
						voter_action[2].label = "Cambio de distrito";
						voter_action[3].label = "Cambio de clave";
						voter_action[4].label = "Eliminar votante";
						action = Menu(voter_action,5).ask();
						if (action==0) {
							Voter v = Voter(atoi(Menu::raw_input("DNI").c_str()), Menu::raw_input("Nombre"), Menu::raw_input("Contraseña"), Menu::raw_input("Direccion"), Menu::raw_input("Distrito"), std::vector<ElectionKey>());
							hash_voter.insertRecord(v.getKey(), &VariableRecord(v.getBytes(), v.getSize())) ? cout << "OK" : cout << "FAILED";
						} else if (action==1) {
							VariableRecord *record;
							Voter v = Voter(atoi(Menu::raw_input("DNI").c_str()), NULL, NULL, NULL, NULL, std::vector<ElectionKey>());
							hash_voter.getRecord(v.getKey(), &record);
							v.setBytes(record->getBytes());
							v.setAddress(Menu::raw_input("Nueva direccion"));
							record->setBytes(v.getBytes(), v.getSize());
							hash_voter.updateRecord(v.getKey(), record);
						} else if (action==2) {
							VariableRecord *record;
							Voter v = Voter(atoi(Menu::raw_input("DNI").c_str()), NULL, NULL, NULL, NULL, std::vector<ElectionKey>());
							hash_voter.getRecord(v.getKey(), &record);
							v.setBytes(record->getBytes());
							v.setDistrict(Menu::raw_input("Nuevo distrito"));
							record->setBytes(v.getBytes(), v.getSize());
							hash_voter.updateRecord(v.getKey(), record);
						} else if (action==3) {
							VariableRecord *record;
							Voter v = Voter(atoi(Menu::raw_input("DNI").c_str()), NULL, NULL, NULL, NULL, std::vector<ElectionKey>());
							hash_voter.getRecord(v.getKey(), &record);
							v.setBytes(record->getBytes());
							v.setPassword(Menu::raw_input("Nueva contraseña"));
							record->setBytes(v.getBytes(), v.getSize());
							hash_voter.updateRecord(v.getKey(), record);
						} else if (action==4) {
							Voter v = Voter(atoi(Menu::raw_input("DNI").c_str()), NULL, NULL, NULL, NULL, std::vector<ElectionKey>());
							hash_voter.removeRecord(v.getKey());
						}
					} else if (action==2) {
						Tree election_tree ("Election.dat", 512, &ElectionMethods(), false);
						option election_action[3];
						election_action[0].label = "Agregar eleccion";
						election_action[1].label = "Eliminar eleccion";
						election_action[2].label = "Imprimir arbol de elecciones";
						action = Menu(election_action,3).ask();
						if (action==0) {
							// agregar eleccion
							std::vector<string> dist_vector;
							while (1) {
								option election_district_action[4];
								election_district_action[0].label = "Asignar distrito";
								election_district_action[1].label = "Eliminar distrito asignado";
								election_district_action[2].label = "Ver distritos asignados";
								election_district_action[3].label = "Seleccion terminada, continuar.";
								action = Menu(election_district_action,4).ask();
								if (action==0) {
									// asignar distrito
									string dist = Menu::raw_input("Distrito");
									dist_vector.push_back(dist);
								} else if (action==1) {
									// eliminar distrito asignado
									string dist = Menu::raw_input("Distrito");
									for(int i = 0; i < dist_vector.size(); i++) {
										if(dist_vector.at(i) == dist){
											dist_vector.erase(dist_vector.begin() + i);
										}
									}
								} else if (action==2) {
									// ver distritos
									for(int i = 0; i < dist_vector.size(); i++) {
										cout << dist_vector.at(i) << endl;
									}
								} else if (action==3) {
									break;
								}
							}
							Election e ((char)atoi(Menu::raw_input("Dia").c_str()), (char)atoi(Menu::raw_input("Mes").c_str()),
									(short)atoi(Menu::raw_input("Anio").c_str()), Menu::raw_input("Cargo"), dist_vector);

							VariableRecord keyRecord(e.getKey(), e.getKeySize());
							VariableRecord dataRecord(e.getBytes(), e.getSize());

							int res = election_tree.insert(&keyRecord, &dataRecord);
							stringstream elec;
							elec << e.getDay(); elec << "/"; elec << e.getMonth(); elec <<  "/"; elec << e.getYear();
							elec << " "; elec << e.getCharge();
							log.write(string("Agregando eleccion ").append(elec.str()), res!=5, true);

						} else if (action == 1) {
							// eliminar eleccion
							Election e ((char)atoi(Menu::raw_input("Dia").c_str()), (char)atoi(Menu::raw_input("Mes").c_str()),
									(short)atoi(Menu::raw_input("Anio").c_str()), Menu::raw_input("Cargo"), std::vector<string>());
							int res = election_tree.remove(e.getKey());
							stringstream elec;
							elec << e.getDay(); elec << "/"; elec << e.getMonth(); elec <<  "/"; elec << e.getYear();
							elec << " "; elec << e.getCharge();
							log.write(string("Eliminado eleccion ").append(elec.str()), res != 4, true);
						} else if (action == 2) {
							// imprimir eleccion
							election_tree.print();
						}

					} else if (action==3) {
//						HashBlockFile charge_hash = HashBlockFile("Charge.dat", 512, new ChargeMethods, new ChargeHashingFunction, 300, false);
						// POR QUE NO ANDA CON LA LINEA DE ARRIBA???
						// Deberia andar así: HashBlockFile charge_hash("Charge.dat", 512, new ChargeMethods, new ChargeHashingFunction, 300, false);
						HashBlockFile charge_hash ("Charge.dat", 512, &ChargeMethods(), &ChargeHashingFunction(), 300, false);
						option charge_action[3];
						charge_action[0].label = "Agregar cargo";
						charge_action[1].label = "Eliminar cargo";
						charge_action[2].label = "Imprimir hash de cargos";
						action = Menu(charge_action,3).ask();
						if (action==0) {
							std::vector<string> subcharges;
							Menu::raw_input("Primero debera seleccionar los subcargos del cargo, continuar");
							while (1) {
								option charge_subcharge_action[4];
								charge_subcharge_action[0].label = "Asignar subcargo";
								charge_subcharge_action[1].label = "Eliminar subcargo asignado";
								charge_subcharge_action[2].label = "Ver subcargos asignados";
								charge_subcharge_action[3].label = "Seleccion terminada, continuar.";
								action = Menu(charge_subcharge_action,4).ask();
								if (action==0) {
									subcharges.push_back(Menu::raw_input("Subcargo"));
								} else if (action==1) {

								} else if (action==2) {
									for (std::vector<string>::iterator i = subcharges.begin(); i != subcharges.end(); ++i) {
										cout << *i << " - ";
									}
									cout << endl;
								} else if (action==3) {
									break;
								}
							}
							Charge c (Menu::raw_input("Nombre del cargo"), subcharges);
							VariableRecord chargerecord(c.getBytes(), c.getSize());
							bool res = charge_hash.insertRecord(c.getKey(), &chargerecord);
							log.write("Agregando cargo", res, true);
						} else if (action==1) {
							bool res = charge_hash.removeRecord(Menu::raw_input("Nombre del cargo").c_str());
							log.write("Eliminando cargo", res, true);
						} else if (action==2) {
							charge_hash.printContent();
						}
					} else if (action==4) {
						Tree electionslist_tree ("ElectionsList.dat", 512, &ElectionsListMethods(), false);
						option list_action[3];
						list_action[0].label = "Agregar lista";
						list_action[1].label = "Eliminar lista";
						list_action[2].label = "Imprimir arbol de listas";
						action = Menu(list_action,3).ask();
						if (action==0) {
							ElectionsList elist (Menu::raw_input("Nombre"), (char)atoi(Menu::raw_input("Dia").c_str()), (char)atoi(Menu::raw_input("Mes").c_str()), (short)atoi(Menu::raw_input("Anio").c_str()), Menu::raw_input("Cargo"));
							VariableRecord elistkey_vr (elist.getKey(), elist.getKeySize());
							VariableRecord elist_vr (elist.getBytes(), elist.getSize());
							int res = electionslist_tree.insert(&elistkey_vr, &elist_vr);
							log.write("Agregando lista", res!=5, true);
						} else if (action==1) {
							ElectionsList elist (Menu::raw_input("Nombre"), (char)atoi(Menu::raw_input("Dia").c_str()), (char)atoi(Menu::raw_input("Mes").c_str()), (short)atoi(Menu::raw_input("Anio").c_str()), Menu::raw_input("Cargo"));
							int res = electionslist_tree.remove(elist.getKey());
							log.write("Eliminando lista", res!=4, true);
						} else if (action==2) {
							electionslist_tree.print();
						}
					} else if (action==5) {
						Tree candidate_tree ("Candidate.dat", 512, &CandidateMethods(), false);
						option candidate_action[3];
						candidate_action[0].label = "Agregar candidato";
						candidate_action[1].label = "Eliminar candidato";
						candidate_action[2].label = "Imprimir arbol de candidatos";
						action = Menu(candidate_action,3).ask();
						if (action==0) {
							Candidate cand ((char)atoi(Menu::raw_input("Dia").c_str()), (char)atoi(Menu::raw_input("Mes").c_str()), (short)atoi(Menu::raw_input("Anio").c_str()), Menu::raw_input("Nombre"), Menu::raw_input("Cargo"), atoi(Menu::raw_input("DNI").c_str()));
							VariableRecord candkey_vr (cand.getKey(), cand.getKeySize());
							VariableRecord cand_vr (cand.getBytes(), cand.getSize());
							int res = candidate_tree.insert(&candkey_vr, &cand_vr);
							log.write("Agregando candidato", res!=5, true);
						} else if (action==1) {
							Candidate cand ((char)atoi(Menu::raw_input("Dia").c_str()), (char)atoi(Menu::raw_input("Mes").c_str()), (short)atoi(Menu::raw_input("Anio").c_str()), Menu::raw_input("Nombre"), Menu::raw_input("Cargo"), atoi(Menu::raw_input("DNI").c_str()));
							int res = candidate_tree.remove(cand.getKey());
							log.write("Eliminando candidato", res!=4, true);
						} else if (action==2) {
							candidate_tree.print();
						}
					} else if (action==6) {
					} else if (action == 7) {
						dataFiles.readConfigFile();
					} else if (action == 8) {
						break;
					}
				}
				break;
		}
	}
	return 0;
}
