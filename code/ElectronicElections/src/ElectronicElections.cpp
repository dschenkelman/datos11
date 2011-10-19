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
#include "Entities/Administrator.h"
#include "Entities/AdministratorMethods.h"
#include "Entities/Count.h"
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
#include "Entities/Count.h"
#include "VariableBlocks/VariableRecord.h"
#include "BPlusTree/Tree.h"
#include "Voting/Log.h"
#include "Voting/Configuration.h"
#include <time.h>
#include <sstream>

#include "Voting/DataFileLoader.h"
#include "Indexes/DistrictElectionsIndex.h"
#include "Indexes/DistrictCounts.h"
#include "Indexes/DistrictCountsMethods.h"
#include "Indexes/CountId.h"

using namespace std;

int run_tests()
{
	//LoadDataFiles dataFiles("config.txt");
	//dataFiles.readConfigFile();

	cout << "***** RUNNING TESTS *****" << endl;

//	srand(time(NULL));
//	SimpleVariableBlockFileTests rlvTest;
//	rlvTest.run();

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
//
//
	cout << "Tree Tests" << endl;
	TreeTests treeTests;
	treeTests.run();
//
//	cout << "Elections List Tests" << endl;
//	ElectionsListTests electionTests;
//	electionTests.run();
//
//	cout << "Candidate Tests" << endl;
//	CandidateTests ct;
//	ct.run();
//
//	cout << "Administrator Tests" << endl;
//	AdministratorTests at;
//	at.run();
//
//	cout << "Count Tests" << endl;
//	CountTests countTests;
//	countTests.run();
//
//	cout << "Charge Tests" << endl;
//	ChargeTests chargeTests;
//	chargeTests.run();

	return 0;
}


void updateCountVoteAmount(Configuration & configuration)
{
    ConfigurationEntry & entry = configuration.getEntry("Count");
    CountMethods countMethods;
    Tree countTree(entry.getDataFileName(), entry.getBlockSize(), &countMethods, false);
    Count c((char)(atoi(Menu::raw_input("Dia").c_str())),
    		(char)(atoi(Menu::raw_input("Mes").c_str())),
    		(short)(atoi(Menu::raw_input("Anio").c_str())),
    		Menu::raw_input("Cargo"),
    		Menu::raw_input("Nombre Lista"),
    		Menu::raw_input("Distrito"),
    		0);
    VariableRecord aux;
    if(!countTree.get(c.getKey(), &aux)){
        cout << "El conteo no existe." << endl;
    }else{
        c.setBytes(aux.getBytes());
        int cant = 0;
        do{
            string cantidadVotos = Menu::raw_input("Nueva cantidad de votos:");
            cant = atoi(cantidadVotos.c_str());
            if(cant < 0){
                cout << "La cantidad no puede ser negativa." << endl;
            }
        }
        while(cant < 0);
        aux.setBytes(c.getBytes(), c.getSize());
        countTree.update(c.getKey(), &aux);
    }

}

int main() // Las pruebas se pueden correr con la opcion 1 muy facilmente, incluso desde eclipse
{			// No impedir el uso normal del sistema
	bool debug = false;

	if (debug) return run_tests();

	Configuration configuration("config.txt");
	configuration.read();
	DataFileLoader dataFiles(configuration);
	dataFiles.loadFiles();

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
					Voting vot(&configuration);
					vot.login(dataFiles.getVoterBlockAmount());
					voting = false;
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
				{
					admin_tree = dataFiles.createAdminFile();
				}
				else
				{
					admin_tree = dataFiles.getAdminFile();
				}
				Administrator admin (user, passwd);
				VariableRecord adminrecord;
				if ( admin_tree->get(admin.getKey(), &adminrecord) )
				{
//					cout << "admin existe" << endl;
					Administrator realadmin("","");
					realadmin.setBytes(adminrecord.getBytes());
					cout << admin.getPassword()<<endl;
					cout <<realadmin.getPassword()<<endl;
					int passcmp = strcmp(admin.getPassword().c_str(), realadmin.getPassword().c_str());
					log.write(string("Logueo de usuario ").append(user), passcmp==0, true);
					if ( passcmp==0 )
					{
						cout << "Bienvenido!" << endl;
					}
					else
					{
						cout << "Contraseña erronea" <<endl; break;
					}
				}
				else if (user=="1" && passwd=="1")
				{
					cout << "Bienvenido al sistema!" <<endl;
				}
				else
				{
					cout << "Usuario erroneo"<<endl; break;
				}
//				else { cout << "no existe el admin"<<endl; }
				while (1) {
					option admin_action[11];
					admin_action[0].label = "Mantener distritos";
					admin_action[1].label = "Mantener votantes";
					admin_action[2].label = "Mantener elecciones";
					admin_action[3].label = "Mantener cargos";
					admin_action[4].label = "Mantener listas";
					admin_action[10].label = "Actualizar conteo";
					admin_action[5].label = "Mantener candidatos";
					admin_action[6].label = "Mantener administradores";
					admin_action[7].label = "Informar resultados";
					admin_action[8].label = "Generar archivos vacios";
					admin_action[9].label = "Volver";
					action = Menu(admin_action,11).ask();
					if (action == 0)
					{
						ConfigurationEntry& entry = configuration.getEntry("District");
						DistrictMethods dm;
						Tree district_tree (entry.getDataFileName(),
								entry.getBlockSize(), &dm, false);
						option district_action[3];
						district_action[0].label = "Agregar distrito";
						district_action[1].label = "Eliminar distrito";
						district_action[2].label = "Volver";
						while (1)
						{
							action = Menu(district_action,3).ask();
							if (action==0)
							{
								District d(Menu::raw_input("Nombre del distrito"));
								VariableRecord record (d.getBytes(), d.getSize());
								int res = district_tree.insert(&record, &record);
								log.write(string("Agregando distrito ").append(d.getName()), res!=5, true);
							}
							else if (action==1)
							{
								District d(Menu::raw_input("Nombre del distrito"));
								int res = district_tree.remove(d.getKey());
								log.write(string("Eliminando distrito ").append(d.getName()), res!=4, true);
							}
							else if (action==2)
							{
								break;
							}
						}
					}
					else if (action == 1)
					{
						ConfigurationEntry& entry = configuration.getEntry("Voter");
						VoterMethods vm;
						VoterHashingFunction vhf;
						HashBlockFile hash_voter(entry.getDataFileName(), entry.getBlockSize(), &vm,
								&vhf, dataFiles.getVoterBlockAmount(), false); // para 28 mil votantes
						option voter_action[4];
						voter_action[0].label = "Agregar votante";
						voter_action[1].label = "Modificar votante";
						voter_action[2].label = "Eliminar votante";
						voter_action[3].label = "Volver";
						while (1)
						{
							action = Menu(voter_action,4).ask();
							if (action==0)
							{
								Voter v(atoi(Menu::raw_input("DNI").c_str()), Menu::raw_input("Nombre"), Menu::raw_input("Contraseña"), Menu::raw_input("Direccion"), Menu::raw_input("Distrito"), std::vector<ElectionKey>());
								VariableRecord vr(v.getBytes(), v.getSize());
								hash_voter.insertRecord(v.getKey(), &vr) ? cout << "OK" : cout << "FAILED";
							}
							else if (action==1)
							{
								// Modificar votante
								VariableRecord *record;
								Voter v(atoi(Menu::raw_input("DNI").c_str()), "", "", "", "", std::vector<ElectionKey>());
								bool found = hash_voter.getRecord(v.getKey(), &record);
								if (found)
								{
									option voter_modification[5];
									voter_modification[0].label = "Cambio de nombre";
									voter_modification[1].label = "Cambio de domicilio";
									voter_modification[2].label = "Cambio de distrito";
									voter_modification[3].label = "Cambio de clave";
									voter_modification[4].label = "Volver";
									while (1)
									{
										action = Menu(voter_modification,5).ask();

										if (action==0)
										{
											v.setBytes(record->getBytes());
											v.setNames(Menu::raw_input("Nuevo nombre"));
											record->setBytes(v.getBytes(), v.getSize());
											hash_voter.updateRecord(v.getKey(), record);

										}
										else if (action==1)
										{
											v.setBytes(record->getBytes());
											v.setAddress(Menu::raw_input("Nueva direccion"));
											record->setBytes(v.getBytes(), v.getSize());
											hash_voter.updateRecord(v.getKey(), record);
										}
										else if (action==2)
										{
											v.setBytes(record->getBytes());
											v.setDistrict(Menu::raw_input("Nuevo distrito"));
											record->setBytes(v.getBytes(), v.getSize());
											hash_voter.updateRecord(v.getKey(), record);
										}
										else if (action==3)
										{
											v.setBytes(record->getBytes());
											v.setPassword(Menu::raw_input("Nueva contraseña"));
											record->setBytes(v.getBytes(), v.getSize());
											hash_voter.updateRecord(v.getKey(), record);
										}
										else if (action==4)
										{
											break;
										}
									}
								}
								else
								{
									cout << endl << "Votante no encontrado!" << endl;
								}
							}
							else if (action==2)
							{
								Voter v(atoi(Menu::raw_input("DNI").c_str()), "", "", "", "", std::vector<ElectionKey>());
								bool removed = hash_voter.removeRecord(v.getKey());
								if (removed)
								{
									cout << endl << "Votante eliminado correctamente!" << endl;
								}
								else
								{
									cout << endl << "Votante no encontrado!" << endl;
								}
							}
							else if (action==3)
							{
								break;
							}
						}
					}
					else if (action==2)	// Mantener elecciones
					{
						ConfigurationEntry& districtElectionsEntry = configuration.getEntry("DistrictElections");
						DistrictElectionsIndex indexFile(districtElectionsEntry.getDataFileName(),
								districtElectionsEntry.getBlockSize(), false);
						ConfigurationEntry& entry = configuration.getEntry("Election");
						ElectionMethods em;
						Tree election_tree(entry.getDataFileName(), entry.getBlockSize(), &em, false);
						while (1)
						{
							option election_action[4];
							election_action[0].label = "Agregar eleccion";
							election_action[1].label = "Modificar eleccion";
							election_action[2].label = "Eliminar eleccion";
							election_action[3].label = "Volver";
							action = Menu(election_action,4).ask();
							if (action==0)
							{
								// agregar eleccion
								std::vector<string> dist_vector;
								while (1)
								{
									option election_district_action[4];
									election_district_action[0].label = "Asignar distrito";
									election_district_action[1].label = "Eliminar distrito asignado";
									election_district_action[2].label = "Ver distritos asignados";
									election_district_action[3].label = "Seleccion terminada, continuar.";
									action = Menu(election_district_action,4).ask();
									if (action==0)
									{
										// asignar distrito
										string dist = Menu::raw_input("Distrito");
										dist_vector.push_back(dist);
									}
									else if (action==1)
									{
										// eliminar distrito asignado
										string dist = Menu::raw_input("Distrito");
										for(unsigned int i = 0; i < dist_vector.size(); i++)
										{
											if(dist_vector.at(i) == dist)
											{
												dist_vector.erase(dist_vector.begin() + i);
											}
										}
									}
									else if (action==2)
									{
										// ver distritos
										for(unsigned int i = 0; i < dist_vector.size(); i++)
										{
											cout << dist_vector.at(i) << endl;
										}
									}
									else if (action==3)
									{
										break;
									}
								}
								if (dist_vector.empty())	// Check if the election has at least one district
								{
									cout << endl << "Debe agregar al menos un distrito!" << endl;
								}
								else
								{
									Election e ((char)atoi(Menu::raw_input("Dia").c_str()), (char)atoi(Menu::raw_input("Mes").c_str()),
											(short)atoi(Menu::raw_input("Anio").c_str()), Menu::raw_input("Cargo"), dist_vector);

									VariableRecord keyRecord(e.getKey(), e.getKeySize());
									VariableRecord dataRecord(e.getBytes(), e.getSize());

									int res = election_tree.insert(&keyRecord, &dataRecord);
									indexFile.indexElection(e);
									stringstream elec;
									elec << (short)e.getDay(); elec << "/"; elec << (short)e.getMonth(); elec <<  "/"; elec << e.getYear();
									elec << " "; elec << e.getCharge();
									log.write(string("Agregando eleccion ").append(elec.str()), res!=NotFound, true);
								}
							}
							else if (action == 1)
							{
								// Modificar eleccion
								Election updatedElection ((char)atoi(Menu::raw_input("Dia").c_str()), (char)atoi(Menu::raw_input("Mes").c_str()),
										(short)atoi(Menu::raw_input("Anio").c_str()), Menu::raw_input("Cargo"));
								VariableRecord record;
								bool found = election_tree.get(updatedElection.getKey(),&record);
								if (!found)
								{
									cout << endl << "Eleccion no encontrada!" << endl;
								}
								else
								{
									updatedElection.setBytes(record.getBytes());
									while (1)
									{
										option election_district_action[4];
										election_district_action[0].label = "Agregar distrito";
										election_district_action[1].label = "Eliminar distrito asignado";
										election_district_action[2].label = "Ver distritos asignados";
										election_district_action[3].label = "Actualizacion terminada, continuar.";
										action = Menu(election_district_action,4).ask();
										if (action==0)
										{
											// asignar distrito
											string dist = Menu::raw_input("Distrito");
											updatedElection.getDistrictList().push_back(dist);
										}
										else if (action==1)
										{
											// eliminar distrito asignado
											string dist = Menu::raw_input("Distrito");
											for(unsigned int i = 0; i < updatedElection.getDistrictList().size(); i++)
											{
												if(updatedElection.getDistrictList().at(i) == dist)
												{
													updatedElection.getDistrictList().erase(updatedElection.getDistrictList().begin() + i);
												}
											}
										}
										else if (action==2)
										{
											// ver distritos
											for(unsigned int i = 0; i < updatedElection.getDistrictList().size(); i++)
											{
												cout << updatedElection.getDistrictList().at(i) << endl;
											}
										}
										else if (action==3)
										{
											break;
										}
									}
								}
								record.setBytes(updatedElection.getBytes(), updatedElection.getSize());
								OpResult result = election_tree.update(updatedElection.getKey(),&record);
								if (result == Updated)
								{
									cout << endl << "Eleccion actualizada correctamente!" << endl;
								}
								else
								{
									cout << endl << "La eleccion no ha sido actualizada!" << endl;
								}
							}
							else if (action == 2)
							{
								// eliminar eleccion
								std::vector<string> districts;
								Election e ((char)atoi(Menu::raw_input("Dia").c_str()), (char)atoi(Menu::raw_input("Mes").c_str()),
								(short)atoi(Menu::raw_input("Anio").c_str()), Menu::raw_input("Cargo"));
								int res = election_tree.remove(e.getKey());
								if (res == NotFound)
								{
									cout << endl << "Eleccion no encontrada!" << endl;
								}
								else
								{
									cout << endl << "Eleccion eliminada correctamente!" << endl;
								}
								indexFile.unIndexElection(e);
								stringstream elec;
								elec << (short) e.getDay(); elec << "/"; elec << (short) e.getMonth(); elec <<  "/"; elec << (short) e.getYear();
								elec << " "; elec << e.getCharge();
								log.write(string("Eliminado eleccion ").append(elec.str()), res != 4, true);

							}
							else if (action == 3)
							{
								break;
							}
						}

					}
					else if (action==3)	// Mantener cargo
					{

						ConfigurationEntry& entry = configuration.getEntry("Charge");
						ChargeMethods cm;
						ChargeHashingFunction chf;
						HashBlockFile charge_hash (entry.getDataFileName(), entry.getBlockSize(),
								&cm, &chf, dataFiles.getChargeBlockAmount(), false);
						option charge_action[4];
						charge_action[0].label = "Agregar cargo";
						charge_action[1].label = "Eliminar cargo";
						charge_action[2].label = "Actualizar cargo";
						charge_action[3].label = "Volver";
						while(1)
						{
							action = Menu(charge_action,4).ask();
							if (action==0)
							{
								std::vector<string> subcharges;
								Menu::raw_input("Primero debera seleccionar los subcargos del cargo, continuar");
								while (1)
								{
									option charge_subcharge_action[4];
									charge_subcharge_action[0].label = "Asignar subcargo";
									charge_subcharge_action[1].label = "Eliminar subcargo asignado";
									charge_subcharge_action[2].label = "Ver subcargos asignados";
									charge_subcharge_action[3].label = "Seleccion terminada, continuar.";
									action = Menu(charge_subcharge_action,4).ask();
									if (action==0)
									{
										subcharges.push_back(Menu::raw_input("Subcargo"));
									}
									else if (action==1)
									{

									}
									else if (action==2)
									{
										for (std::vector<string>::iterator i = subcharges.begin(); i != subcharges.end(); ++i)
										{
											cout << *i << " - ";
										}
										cout << endl;
									}
									else if (action==3)
									{
										break;
									}
								}
								Charge c (Menu::raw_input("Nombre del cargo"), subcharges);
								VariableRecord chargerecord(c.getBytes(), c.getSize());
								bool res = charge_hash.insertRecord(c.getKey(), &chargerecord);
								log.write("Agregando cargo", res, true);
							}
							else if (action==1)
							{
								Charge c (Menu::raw_input("Nombre del cargo"), vector<string>());
								bool res = charge_hash.removeRecord(c.getKey());
								log.write("Eliminando cargo: '" + c.getCharge() + "' ", res, true);
							}
							else if (action==2)
							{
								string cargo = Menu::raw_input("Nombre del cargo");
								vector<string> subCharges;
								Charge c(cargo, subCharges);
								VariableRecord* r = NULL;
								bool success = false;

								if(!charge_hash.getRecord(c.getKey(), &r))
								{
									cout << "El cargo no esta registrado" << endl;
								}
								else
								{
									c.setBytes(r->getBytes());
									charge_action[0].label = "Agregar sub-cargo";
									charge_action[1].label = "Eliminar sub-cargo";
									action = Menu(charge_action,2).ask();
									string subcargo = Menu::raw_input("Nombre del sub-cargo");
									if (action == 0)
									{
										// agregar sub-cargo
										c.getChargeList().push_back(subcargo);
										cout << "Agregado correctamente" << endl;
										success = true;
									}
									else if (action == 1)
									{
										// borrar sub-cargo
										vector<string> buffer;
										while(!c.getChargeList().empty())
										{
											string charge = c.getChargeList().at(c.getChargeList().size() - 1);
											c.getChargeList().pop_back();
											if (charge == subcargo)
											{
												cout << "Eliminado correctamente" << endl;
												success = true;
												break;
											}
											buffer.push_back(charge);
										}

										while(!buffer.empty())
										{
											string charge = buffer.at(buffer.size() - 1);
											buffer.pop_back();
											c.getChargeList().push_back(charge);
										}
									}
									if (success)
									{
										VariableRecord aux;
										aux.setBytes(c.getBytes(), c.getSize());
										charge_hash.updateRecord(c.getKey(),&aux);
									}
								}

								if (r != NULL)
								{
									delete r;
								}
							}
							else if (action == 3)
							{
								// volver
								break;
							}
						}

					}
					else if (action==4)	// Mantener Listas
					{
						// Election Lists
						ConfigurationEntry& entry = configuration.getEntry("Election");
						ElectionsListMethods elm;
						Tree electionslist_tree(entry.getDataFileName(), entry.getBlockSize(),
								&elm, false);
						option list_action[3];
						list_action[0].label = "Agregar lista";
						list_action[1].label = "Eliminar lista";
						list_action[2].label = "Volver";
						while(1)
						{
							action = Menu(list_action,3).ask();
							if (action==0)
							{
								ElectionsList elist (Menu::raw_input("Nombre"), (char)atoi(Menu::raw_input("Dia").c_str()), (char)atoi(Menu::raw_input("Mes").c_str()), (short)atoi(Menu::raw_input("Anio").c_str()), Menu::raw_input("Cargo"));
								VariableRecord elistkey_vr (elist.getKey(), elist.getKeySize());
								VariableRecord elist_vr (elist.getBytes(), elist.getSize());
								int res = electionslist_tree.insert(&elistkey_vr, &elist_vr);
								log.write("Agregando lista", res!=5, true);
							}
							else if (action==1)
							{
								ElectionsList elist (Menu::raw_input("Nombre"), (char)atoi(Menu::raw_input("Dia").c_str()), (char)atoi(Menu::raw_input("Mes").c_str()), (short)atoi(Menu::raw_input("Anio").c_str()), Menu::raw_input("Cargo"));
								int res = electionslist_tree.remove(elist.getKey());
								log.write("Eliminando lista " + elist.getName(), res!=4, true);
							}
							else if (action==2)
							{
								break;
							}
						}
					}
					else if (action==5)	// Mantener candidatos
					{
						// Arbol candidatos
						ConfigurationEntry& entry = configuration.getEntry("Candidate");
						CandidateMethods cm;
						Tree candidate_tree (entry.getDataFileName(), entry.getBlockSize(), &cm, false);

						// Hash votantes
						ConfigurationEntry& voterEntry = configuration.getEntry("Voter");
						VoterMethods vm;
						VoterHashingFunction vhf;
						HashBlockFile hash_voter(voterEntry.getDataFileName(), voterEntry.getBlockSize(), &vm,
								&vhf, dataFiles.getVoterBlockAmount(), false); // para 28 mil votantes

						// Election Lists
						ConfigurationEntry& electionEntry = configuration.getEntry("Election");
						ElectionsListMethods elm;
						Tree electionslist_tree(electionEntry.getDataFileName(), electionEntry.getBlockSize(),
								&elm, false);

						option candidate_action[3];
						candidate_action[0].label = "Agregar candidato";
						candidate_action[1].label = "Eliminar candidato";
						candidate_action[2].label = "Volver";
						while(1)
						{
							action = Menu(candidate_action,3).ask();
							if (action==0)
							{
								Candidate cand ((char)atoi(Menu::raw_input("Dia").c_str()), (char)atoi(Menu::raw_input("Mes").c_str()), (short)atoi(Menu::raw_input("Anio").c_str()), Menu::raw_input("Nombre"), Menu::raw_input("Cargo"), atoi(Menu::raw_input("DNI").c_str()));
								VariableRecord candkey_vr (cand.getKey(), cand.getKeySize());
								VariableRecord cand_vr (cand.getBytes(), cand.getSize());
								int res = candidate_tree.insert(&candkey_vr, &cand_vr);
								log.write("Agregando candidato", res!=5, true);
							}
							else if (action==1)
							{
								int dni = atoi(Menu::raw_input("DNI").c_str());
								/*VariableRecord *record;
								Voter v(dni, "", "", "", "", std::vector<ElectionKey>());
								bool found = hash_voter.getRecord(v.getKey(), &record);
								if (!found)
								{
									cout << endl << "El candidato no pertenece al padron";
								}*/
								Candidate cand ((char)atoi(Menu::raw_input("Dia").c_str()), (char)atoi(Menu::raw_input("Mes").c_str()), (short)atoi(Menu::raw_input("Anio").c_str()), Menu::raw_input("Nombre"), Menu::raw_input("Cargo"), dni);
								int res = candidate_tree.remove(cand.getKey());
								log.write("Eliminando candidato", res!=4, true);
							}
							else if (action==2)
							{
								break;
							}
						}

					}
					else if (action==6)
					{
						option administrator_action[3];
						administrator_action[0].label = "Agregar administrador";
						administrator_action[1].label = "Eliminar administrador";
						administrator_action[2].label = "Volver";
						while(1)
						{
							action = Menu(administrator_action,3).ask();
							if (action==0)
							{
								Administrator newadmin(Menu::raw_input("Usuario"), Menu::raw_input("Contraseña"));
								VariableRecord adminkey_vr (newadmin.getKey(), newadmin.getKeySize());
								VariableRecord admin_vr (newadmin.getBytes(), newadmin.getSize());
								int res = admin_tree->insert(&adminkey_vr, &admin_vr);
								cout << "KEY: "<<newadmin.getKey() << endl;
								log.write(string("Agregando administrador ").append(newadmin.getName()), res!=5, true);
							}
							else if (action==1)
							{
								Administrator remadmin(Menu::raw_input("Usuario"), "");
								int res = admin_tree->remove(remadmin.getKey());
								log.write(string("Eliminando administrador ").append(remadmin.getName()), res!=4, true);
							}
							else if (action==2)
							{
								break;
							}
						}

					}
					else if (action==7)
					{
						option list_action[4];
						list_action[0].label = "Reporte por Distrito";
						list_action[1].label = "Reporte por Lista";
						list_action[2].label = "Reporte por Eleccion";
						list_action[3].label = "Volver";
						while(1)
						{
							action = Menu(list_action,4).ask();
							if(action == 0)
							{
								//District report
								std::string district = Menu::raw_input("Distrito");
								DistrictCounts indexDistrict(district);
								ConfigurationEntry& entry = configuration.getEntry("DistrictCounts");
								Tree index_districtCount (entry.getDataFileName(), entry.getBlockSize(), &DistrictCountsMethods(), false);
								VariableRecord countIdRecord;
								if(! index_districtCount.get(indexDistrict.getKey(), &countIdRecord) )
								{
									std::cout << "No election found in: " << indexDistrict.getDistrict() << endl;
									log.write(string("Reporte distrito invalido: ").append(indexDistrict.getDistrict()),false, true );//lista invalida
									break;
								}
								indexDistrict.setBytes(countIdRecord.getBytes());
								std::vector<CountId> countsId = indexDistrict.getCountIds();
								CountId countId = countsId.at(0);//first count id
								int totalCountId = countsId.size();
								Count count(countId.getDay(), countId.getMonth(), countId.getYear(), countId.getCharge(), countId.getListName(), indexDistrict.getDistrict(), 0);
								entry = configuration.getEntry("Count");
								Tree list_count(entry.getDataFileName(), entry.getBlockSize(),&CountMethods(), false);
								VariableRecord countRecord;

								list_count.get(count.getKey(), &countRecord);
								count.setBytes(countRecord.getBytes());
								int listVotes= count.getQuantity();
								std::vector<string> listNames;
								listNames.push_back(count.getListName());
								for(int i=1;i<totalCountId;i++)
								{
									countId = countsId.at(i);
									Count nextCount(countId.getDay(), countId.getMonth(), countId.getYear(), countId.getCharge(), countId.getListName(), indexDistrict.getDistrict(), 0);

									if(count.getDay() == nextCount.getDay() && count.getMonth() == nextCount.getMonth() && count.getYear() == nextCount.getYear() && count.getCharge()== nextCount.getCharge())
									{
										//same election
										list_count.get(nextCount.getKey(), &countRecord);
										nextCount.setBytes(countRecord.getBytes());
										if(nextCount.getQuantity() == listVotes )
										{
											listNames.push_back(nextCount.getListName());
										}
										else if(nextCount.getQuantity() > listVotes)
										{
											listNames.clear();
											listNames.push_back(nextCount.getListName());
										}
									}
									else
									{
										//change of election. print previous one
										std::cout << "Eleccion: " << count.getCharge()<<" "<< count.getDay()<<"-"<<count.getMonth()<<"-"<<count.getYear() << endl;
										//log.write(string("Eleccion: ").append(indexDistrict.getDistrict()), true, true);
										for(int j=0;j< listNames.size();j++)
										{
											std::cout << "--Lista: " << listNames.at(j) << ", votos: " << listVotes << endl;
										}
										listNames.clear();
										listVotes = 0;
										count.setBytes(nextCount.getBytes());//saving new election
									}
								}
								log.write(string("Finalizado Reporte por Distrito: ").append(indexDistrict.getDistrict()), true, true);
							}
							if(action == 1)
							{
								ConfigurationEntry& entry = configuration.getEntry("Candidate");
								CandidateMethods cm;
								Tree candidate_tree (entry.getDataFileName(), entry.getBlockSize(), &cm, false);
								Candidate candidate((char)atoi(Menu::raw_input("Dia").c_str()), (char)atoi(Menu::raw_input("Mes").c_str()), (short)atoi(Menu::raw_input("Anio").c_str()), Menu::raw_input("Nombre Lista"), "", 0);
								VariableRecord candRecord;
								candidate_tree.get(candidate.getKey(), &candRecord);
								if(candRecord.getSize() == 0)
								{
									std::cout << "Invalid list. List was not found." << endl;
									log.write(string("Reporte Lista invalida: ").append(candidate.getName()),false, true );//lista invalida
									break;
								}
								candidate.setBytes(candRecord.getBytes());//got candidate of list
								//get name from dni
								entry = configuration.getEntry("Voter");
								VoterMethods vm;
								VoterHashingFunction vhf;
								HashBlockFile hash_voter(entry.getDataFileName(), entry.getBlockSize(),&vm, &vhf,dataFiles.getVoterBlockAmount(), false);
								VariableRecord* voterRecord = NULL;
								Voter voter(0, "invalid", "invalid", "invalid", "invalid", std::vector<ElectionKey>());
								int dni = candidate.getDni();
								char keyDni[sizeof(int)];
								memcpy(keyDni, &dni, sizeof(int));
								hash_voter.getRecord(keyDni, &voterRecord);
								voter.setBytes(voterRecord->getBytes());
								if(voterRecord!= NULL){ delete voterRecord; }

								//get districts from election
								entry = configuration.getEntry("Election");
								CountMethods countMethods;
								Tree electionTree(entry.getDataFileName(), entry.getBlockSize(),&countMethods, false);
								Election election(candidate.getDay(), candidate.getMonth(), candidate.getYear(), candidate.getCharge() );
								Election nextElec(candidate.getDay(), candidate.getMonth(), candidate.getYear(), candidate.getCharge());
								VariableRecord electionRec;
								int res = electionTree.get(election.getKey(), &electionRec);

								entry = configuration.getEntry("Count");
								Tree list_count(entry.getDataFileName(), entry.getBlockSize(),&countMethods, false);
								VariableRecord countRecord;
								std::cout << "Lista: " << candidate.getDay() << candidate.getMonth() << candidate.getYear()<< candidate.getName() << endl;
								std::cout << "Candidato: " << candidate.getCharge() << ", " << voter.getNames() << endl;
								log.write(string("Reportando lista ").append(candidate.getName()), true, true);
								int totalCount = 0;
								char votes[sizeof(int)];
								int countVotes;

								Count count(candidate.getDay(), candidate.getMonth(), candidate.getYear(), candidate.getCharge(), candidate.getName(),"", 0);
								list_count.get(count.getKey(),&countRecord );
								if(countRecord.getSize() == 0)
								{
									std::cout << "No hay votos registrados para lista: " << count.getListName() << endl;
									log.write(string("Lista sin votos: ").append(candidate.getName()), false, true);
									break;
								}
								count.setBytes(countRecord.getBytes());
								Count nextCount(candidate.getDay(), candidate.getMonth(), candidate.getYear(), candidate.getCharge(), candidate.getName(),"", 0);
								while(strcmp(count.getListName().c_str(), nextCount.getListName().c_str())==0 ){
									std::string district = count.getDistrict();
									countVotes = count.getQuantity();
									std::cout << "Distrito: " << district << endl;
									std::cout << "Votos: " << countVotes << endl;
									memcpy(votes, &(countVotes), sizeof(int));
									log.write(string("Distrito: ").append(district).append(", Votos: ").append(string(votes)), true, true);
									list_count.getNext(&countRecord);//si es el ultimo, obtengo null?
									nextCount.setBytes(countRecord.getBytes());
									if(countRecord.getSize() == 0)
									{
										break;//last Record found
									}
									totalCount+= countVotes;
								}
								char total[sizeof(int)];
								memcpy(total, &totalCount, sizeof(int));
								std::cout << "Votos Totales: " << totalCount << endl;
								log.write(string("Votos Totales Lista: ").append(candidate.getName()).append(", ").append(string(total)), true, true);
							}
							if(action == 2)
							{
								//report by election
								std::string dia = Menu::raw_input("Dia");
								std::string mes = Menu::raw_input("Mes");
								std::string anio = Menu::raw_input("Anio");
								std::string charge = Menu::raw_input("Cargo");
								ConfigurationEntry& entry = configuration.getEntry("ElectionList");
								Tree list_tree (entry.getDataFileName(), entry.getBlockSize(), &ElectionsListMethods(), false);
								ElectionsList list("", (char)atoi(dia.c_str()), (char)atoi(mes.c_str()), (short)atoi(anio.c_str()), charge);
								ElectionsList nextList("", (char)atoi(dia.c_str()), (char)atoi(mes.c_str()), (short)atoi(anio.c_str()), charge);
								VariableRecord listRecord;

								list_tree.get(list.getKey(), &listRecord);
								if(listRecord.getSize() == 0)
								{//reached end of file
									std::cout << "Invalid Election. Election was not found." << endl;
									log.write(string("Reporte Eleccion invalida: ").append(dia +"-"+ mes +"-"+ anio),false, true );//lista invalida
									break;
								}
								list.setBytes(listRecord.getBytes());
								if(list.getDay() != nextList.getDay() || list.getMonth() != nextList.getMonth() ||list.getYear() != nextList.getYear() || list.getCharge()!= nextList.getCharge())
								{//got next election
									std::cout << "Invalid Election. Election was not found." << endl;
									log.write(string("Reporte Eleccion invalida: ").append(dia +"-"+ mes +"-"+ anio),false, true );//lista invalida
									break;
								}
								Count count(list.getDay(), list.getMonth(), list.getYear(), list.getCharge(), list.getName(),"", 0);
								entry = configuration.getEntry("Count");
								Tree list_count(entry.getDataFileName(), entry.getBlockSize(),&CountMethods(), false);
								VariableRecord countRecord;
								Count nextCount(list.getDay(), list.getMonth(), list.getYear(), list.getCharge(), list.getName(),"", 0);

								std::cout << "Eleccion: " << dia << "-" << mes << "-" << anio << endl;
								log.write(string("Reportando Eleccion: ").append(dia +"-"+ mes +"-"+ anio+", " +list.getCharge()),true, true );
								char votos[sizeof(int)];
								list_count.get(count.getKey(), &countRecord);
								if(countRecord.getSize() == 0)
								{
									//No record, end of file
									std::cout << "No hay votos registrados para eleccion: " << dia << "-" << mes << "-" << anio << ", " << count.getCharge()<< endl;
									log.write(string("Eleccion sin votos: ").append(dia +"-"+ mes +"-"+ anio+", " +list.getCharge()), false, true);
									break;
								}
								nextCount.setBytes(countRecord.getBytes());
								while(count.getDay() == nextCount.getDay() && count.getMonth() == nextCount.getMonth() && count.getYear() == nextCount.getYear() && list.getCharge()== nextList.getCharge())
								{
									int listVotes = 0;
									bool lastRecord = false;
									count.setBytes(nextCount.getBytes());

									while( strcmp(count.getListName().c_str(), nextCount.getListName().c_str()) == 0 )
									{
										//acumulate votes for one list in each district
										listVotes+= nextCount.getQuantity();
										list_count.getNext(&countRecord);
										if(countRecord.getSize() == 0)
										{
											lastRecord = true;
											break;
										}
										nextCount.setBytes(countRecord.getBytes());
									}
									memcpy(votos, &listVotes, sizeof(int));
									std::cout << "Lista: " << count.getListName() << ", Votos: " << listVotes << endl;
									log.write(string("Lista: ").append(count.getListName()).append(", votos totales: ").append(string(votos)),true, true );

									if(lastRecord)
									{
										break;//no more districts
									}
								}
								std::cout << "Finalizado reporte Eleccion: " << dia << "-" << mes << "-" << anio << ", " << count.getCharge() << endl;
								log.write(string("Finalizado reporte Eleccion: ").append(dia +"-"+ mes +"-"+ anio+", "+list.getCharge()),true, true );

							}//close option for election report
							else if (action==3)
							{
								break;
							}
						}
					}
					else if (action == 8)
					{
						dataFiles.loadFiles();
					}
					else if (action == 9)
					{
						break;
					}
					else if (action == 10)
					{
					    updateCountVoteAmount(configuration);
					}

				}
				delete admin_tree;
				break;
		}
	}
	return 0;
}
