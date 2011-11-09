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
#include "RSA/RSACipherTests.h"
#include "RSA/RSAKeySetTests.h"
#include "Helpers/ValidationTests.h"
#include "Helpers/Validation.h"
#include <time.h>
#include <sstream>
#include "Voting/DataFileLoader.h"
#include "Indexes/DistrictElectionsIndex.h"
#include "Indexes/DistrictCounts.h"
#include "Indexes/DistrictCountsMethods.h"
#include "Indexes/CountId.h"
#include <algorithm>
#include <sys/stat.h>
#include <string.h>
#include "RSA/RSACipherTests.h"
#include "Vigenere/VigenereCipher.h"
#include "Vigenere/Kasisky.h"

using namespace std;

bool countIdCmp (CountId c1,CountId c2)
{
	if (c1.getYear() < c2.getYear())
	{
		return false;
	}
	else if (c1.getYear() == c2.getYear())
	{
		if (c1.getMonth() < c2.getMonth())
		{
			return false;
		}
		else if (c1.getMonth() == c2.getMonth())
		{
			if (c1.getDay() < c2.getDay())
			{
				return false;
			}
			else if(c1.getDay() == c2.getDay())
			{
				if (c1.getCharge() < c2.getCharge())
				{
					return false;
				}
				else if(c1.getCharge() == c2.getCharge())
				{
					if(c1.getListName() < c2.getListName())
					{
						return false;
					}
				}
			}
		}
	}

	return true;
}

string askPassword()
{
	string password = Menu::raw_input("Password para el archivo. (caracteres imprimibles)");

	bool notPrintable = false;
	for(unsigned int i = 0; i < password.length(); i++)
	{
		notPrintable = !isprint(password[i]);
	}

	while(notPrintable)
	{
		password = Menu::raw_input("Password para el archivo. (caracteres imprimibles)");
		for(unsigned int i = 0; i < password.length(); i++)
		{
			notPrintable = !isprint(password[i]);
		}
	}

	return password;
}

void saveReport(stringstream& report)
{
	string answer = Menu::raw_input("Guardar el reporte (S|N)");
	while (answer != "S" && answer != "s" && answer != "n" && answer != "N")
	{
		answer = Menu::raw_input("Guardar el reporte (S|N)");
	}

	if (answer == "s" || answer == "S")
	{
		VigenereCipher cipher;
		string fileName = Menu::raw_input("Nombre archivo");
		string password = askPassword();
		string reportString = report.str();
		string criptogram = cipher.encript(reportString, password);
		string directory = "Files/Reports/";
		mkdir(directory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		string fullName = directory + fileName;
		filebuf fb;
		fb.open (fullName.c_str(),ios::out);
		ostream os(&fb);
		os << criptogram;
		fb.close();
	}
}

void recoverReport()
{
	string fileName = Menu::raw_input("Nombre archivo");
	string directory = "Files/Reports/";
	string fullName = directory + fileName;
	ifstream reportFile(fullName.c_str());
	if (reportFile.is_open())
	{
		VigenereCipher cipher;
		string password = askPassword();
		reportFile.seekg(0, std::ios::end);
		long length = reportFile.tellg();
		reportFile.seekg(0, std::ios::beg);
		char buffer[length + 1];
		memset(buffer, 0, length + 1);
		reportFile.read(buffer, length);
		string reportString = buffer;
		string message = cipher.decript(reportString, password);
		cout << message << endl;
	}
	else
	{
		cout << "El reporte no existe." << endl;
	}
}

void crackReport()
{
	string fileName = Menu::raw_input("Nombre archivo");
	string directory = "Files/Reports/";
	string fullName = directory + fileName;
	ifstream reportFile(fullName.c_str());
	if (reportFile.is_open())
	{
		reportFile.seekg(0, std::ios::end);
		long length = reportFile.tellg();
		reportFile.seekg(0, std::ios::beg);
		char buffer[length + 1];
		memset(buffer, 0, length + 1);
		reportFile.read(buffer, length);
		string reportString = buffer;
		Kasisky k;
		k.attack(reportString, 3);
	}
	else
	{
		cout << "El reporte no existe." << endl;
	}
}

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
//	cout << "Tree Tests" << endl;
//	TreeTests treeTests;
//	treeTests.run();
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

//	cout << "RSA Cipher Tests" << endl;
//	RSACipherTests rsaCipherTests;
//	rsaCipherTests.run();

	cout << "RSAKeySet Tests" << endl;
	RSAKeySetTests rsaKeySetTests;
	rsaKeySetTests.run();

//	cout << "Validation Tests" << endl;
//	ValidationTests validationTests;
//	validationTests.run();

	return 0;
}

void enterValidDate(short* year, char* month, char* day)
{
	cout << "Se solicita la fecha de la eleccion, en formato numerico; primero dia, luego mes, y finalmente anio." << endl;
	char auxDay;
	char auxMonth;
	short auxYear;

	Validation validation;

	bool isValid = true;

	do
	{
		auxDay = (char) atoi(Menu::raw_input("Dia").c_str());
		auxMonth = (char) atoi(Menu::raw_input("Mes").c_str());
		auxYear = (short) atoi(Menu::raw_input("Anio").c_str());

		isValid = validation.isValidDate(auxYear, auxMonth, auxDay);

		if(!isValid)
		{
			cout << "Fecha invalida. Ingrese nuevamente." << endl << endl;
		}

	} while (!isValid);

	*year = auxYear;
	*month = auxMonth;
	*day = auxDay;
}

void updateCountVoteAmount(Configuration & configuration)
{
    ConfigurationEntry & entry = configuration.getEntry("Count");
    CountMethods countMethods;
    Tree countTree(entry.getDataFileName(), entry.getBlockSize(), &countMethods, false);
    char day; char month; short year;

    cout << "Se solicita la fecha de la eleccion." << endl;
    enterValidDate(&year, &month, &day);

    Count c(day,
    		month,
    		year,
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
        c.setQuantity(cant);
        aux.setBytes(c.getBytes(), c.getSize());
        countTree.update(c.getKey(), &aux);
    }

}

int main()
{
	bool debug = true;

	if (debug) return run_tests();

	Configuration configuration("Files/config.txt");
	configuration.read();
	DataFileLoader dataFiles(configuration);
	dataFiles.calculateBlockAmounts();

	Log log;
	log.write("Iniciando sistema", true, true);

	option login_type[2];
	login_type[0].label = "Log-In";
	login_type[1].label = "Quit";
	while (1)
	{
		int action = Menu(login_type,2).ask();
		switch (action)
		{
			case 1:
				return 0;
				break;
			case 0:
				string user = Menu::raw_input("User");
				string passwd = Menu::raw_input("Password");
				ConfigurationEntry& entry = configuration.getEntry("Administrator");
				AdministratorMethods am;
				Tree admin_tree (entry.getDataFileName(),
						entry.getBlockSize(), &am, false);
				Administrator admin (user, passwd);
				VariableRecord adminrecord;
				if ( admin_tree.get(admin.getKey(), &adminrecord) )
				{
					Administrator realadmin("","");
					realadmin.setBytes(adminrecord.getBytes());
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
				else { cout << "no existe el admin"<<endl; break;}
				while (1) {
					option admin_action[14];
					admin_action[0].label = "Poblar archivos";
					admin_action[1].label = "Generar votos";
					admin_action[2].label = "Mantener distritos";
					admin_action[3].label = "Mantener votantes";
					admin_action[4].label = "Mantener elecciones";
					admin_action[5].label = "Mantener cargos";
					admin_action[6].label = "Mantener listas";
					admin_action[7].label = "Mantener candidatos";
					admin_action[8].label = "Mantener administradores";
					admin_action[9].label = "Informar resultados";
					admin_action[10].label = "Actualizar conteo";
					admin_action[11].label = "Recuperar reporte";
					admin_action[12].label = "Desencriptar reporte";
					admin_action[13].label = "Volver";
					action = Menu(admin_action,14).ask();
					if (action == 0)
					{
						log.write("Comenzando carga archivos.", true, true);
						dataFiles.loadFiles();
						log.write("Finalizando carga archivos.", true, true);
					}

					else if (action == 1)
					{
						bool voting = true;
						Voting vot(&configuration);
						vot.login(dataFiles.getVoterBlockAmount());
						voting = false;
					}

					else if (action == 2)
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
						{//district_tree.print();
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
					else if (action == 3)
					{
						// Hash votantes
						ConfigurationEntry& entry = configuration.getEntry("Voter");
						VoterMethods vm;
						VoterHashingFunction vhf;
						HashBlockFile hash_voter(entry.getDataFileName(), entry.getBlockSize(), &vm,
								&vhf, dataFiles.getVoterBlockAmount(), false); // para 28 mil votantes

						// Distritos
						ConfigurationEntry& districtEntry = configuration.getEntry("District");
						DistrictMethods dm;
						Tree district_tree (districtEntry.getDataFileName(),
								districtEntry.getBlockSize(), &dm, false);

						option voter_action[4];
						voter_action[0].label = "Agregar votante";
						voter_action[1].label = "Modificar votante";
						voter_action[2].label = "Eliminar votante";
						voter_action[3].label = "Volver";
						while (1)
						{//hash_voter.printContent();
							action = Menu(voter_action,4).ask();
							if (action==0)
							{
								District district(Menu::raw_input("Distrito"));
								VariableRecord r;
								bool found = district_tree.get(district.getKey(),&r);
								if (!found)
								{
									cout << endl << "Distrito invalido!" << endl;
								}
								else
								{
									string dnistr = Menu::raw_input("DNI").c_str();
									Voter v(atoi(dnistr.c_str()), Menu::raw_input("Nombre"), Menu::raw_input("Contraseña"), Menu::raw_input("Direccion"), district.getName(), std::vector<ElectionKey>());
									VariableRecord vr(v.getBytes(), v.getSize());
									bool res = hash_voter.insertRecord(v.getKey(), &vr);
									log.write(string("Agregando votante ").append(dnistr), res, true);
								}
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
											District district(Menu::raw_input("Nuevo Distrito"));
											VariableRecord r;
											bool districtFound = district_tree.get(district.getKey(),&r);
											if (!districtFound)
											{
												cout << endl << "Distrito invalido!" << endl;
											}
											else
											{
												v.setBytes(record->getBytes());
												v.setDistrict(district.getName());
												record->setBytes(v.getBytes(), v.getSize());
												hash_voter.updateRecord(v.getKey(), record);
											}
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
					else if (action==4)	// Mantener elecciones
					{
						ConfigurationEntry& districtElectionsEntry = configuration.getEntry("DistrictElections");
						DistrictElectionsIndex indexFile(districtElectionsEntry.getDataFileName(),
								districtElectionsEntry.getBlockSize(), false);

						// Arbol elecciones
						ConfigurationEntry& entry = configuration.getEntry("Election");
						ElectionMethods em;
						Tree election_tree(entry.getDataFileName(), entry.getBlockSize(), &em, false);

						// Distritos
						ConfigurationEntry& districtEntry = configuration.getEntry("District");
						DistrictMethods dm;
						Tree district_tree (districtEntry.getDataFileName(),
								districtEntry.getBlockSize(), &dm, false);

						// Hash cargos
						ConfigurationEntry& chargeEntry = configuration.getEntry("Charge");
						ChargeMethods cm;
						ChargeHashingFunction chf;
						HashBlockFile charge_hash (chargeEntry.getDataFileName(), chargeEntry.getBlockSize(),
								&cm, &chf, dataFiles.getChargeBlockAmount(), false);


						while (1)
						{//election_tree.print();
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
										District district(dist);
										VariableRecord r;
										bool found = district_tree.get(district.getKey(),&r);
										if (!found)
										{
											cout << endl << "Distrito invalido!" << endl;
											cout << "Los distritos disponibles son:" << endl;
											district_tree.print();
										}
										else
										{
											dist_vector.push_back(dist);
										}
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
								if (dist_vector.empty())		// Check if the election has at least one district
								{
									cout << endl << "Debe agregar al menos un distrito!" << endl;
								}
								else
								{
									char day;
									char month;
									short year;

									enterValidDate(&year, &month, &day);
									Election e (day, month, year, Menu::raw_input("Cargo"), dist_vector);

									Charge c (e.getCharge(), vector<string>());
									VariableRecord *r;
									bool chargeFound = charge_hash.getRecord(c.getKey(),&r);

									OpResult res;
									if (chargeFound)
									{
										VariableRecord keyRecord(e.getKey(), e.getKeySize());
										VariableRecord dataRecord(e.getBytes(), e.getSize());
										res = election_tree.insert(&keyRecord, &dataRecord);
										indexFile.indexElection(e);
									}
									else
									{
										cout << endl << "Cargo no encontrado!" << endl;
										res = NotFound;
									}
									stringstream elec;
									elec << (short)e.getDay(); elec << "/"; elec << (short)e.getMonth(); elec <<  "/"; elec << e.getYear();
									elec << " "; elec << e.getCharge();
									log.write(string("Agregando eleccion ").append(elec.str()), res!=NotFound, true);
								}
							}
							else if (action == 1)
							{
								// Modificar eleccion
								char day;
								char month;
								short year;

								enterValidDate(&year, &month, &day);

								Election updatedElection (day, month, year, Menu::raw_input("Cargo"));
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
											string dist = Menu::raw_input("Nuevo Distrito");
											District district(dist);
											VariableRecord r;
											bool found = district_tree.get(district.getKey(),&r);
											if (!found)
											{
												cout << endl << "Distrito invalido!" << endl;
											}
											else
											{
												updatedElection.getDistrictList().push_back(dist);
											}
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

							}
							else if (action == 2)
							{
								// eliminar eleccion
								std::vector<string> districts;
								char day;
								char month;
								short year;

								enterValidDate(&year, &month, &day);

								Election e (day, month, year, Menu::raw_input("Cargo"));
								OpResult res = election_tree.remove(e.getKey());
								if (res == NotFound)
								{
									cout << endl << "Eleccion no encontrada!" << endl;
								}
								else
								{
									cout << endl << "Eleccion eliminada correctamente!" << endl;
									indexFile.unIndexElection(e);
								}
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
					else if (action==5)	// Mantener cargo
					{
						// Hash cargos
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
						{//charge_hash.printContent();
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
					else if (action==6)	// Mantener Listas
					{
						// Election Lists
						ConfigurationEntry& entry = configuration.getEntry("ElectionList");
						ElectionsListMethods elm;
						Tree electionslist_tree(entry.getDataFileName(), entry.getBlockSize(),
								&elm, false);

						// Elections
						ConfigurationEntry& electionEntry = configuration.getEntry("Election");
						ElectionMethods em;
						Tree election_tree(electionEntry.getDataFileName(), electionEntry.getBlockSize(), &em, false);

						option list_action[3];
						list_action[0].label = "Agregar lista";
						list_action[1].label = "Eliminar lista";
						list_action[2].label = "Volver";
						while(1)
						{//electionslist_tree.print();
							action = Menu(list_action,3).ask();
							if (action==0)
							{
								char day;
								char month;
								short year;
								string name = Menu::raw_input("Nombre");

								enterValidDate(&year, &month, &day);
								ElectionsList elist (name, day, month, year, Menu::raw_input("Cargo"));

								Election e (elist.getDay(), elist.getMonth(),
										elist.getYear(), elist.getCharge());

								VariableRecord r;
								bool found = election_tree.get(e.getKey(),&r);
								OpResult res;
								if (found)
								{
									VariableRecord elistkey_vr (elist.getKey(), elist.getKeySize());
									VariableRecord elist_vr (elist.getBytes(), elist.getSize());
									res = electionslist_tree.insert(&elistkey_vr, &elist_vr);
								}
								else
								{
									cout << endl << "Eleccion no encontrada" << endl;
									res = NotFound;
								}
								log.write("Agregando lista " + elist.getName(), res!=NotFound, true);
							}
							else if (action==1)
							{
								char day;
								char month;
								short year;

								enterValidDate(&year, &month, &day);

								ElectionsList elist (Menu::raw_input("Nombre"), day, month, year, Menu::raw_input("Cargo"));
								int res = electionslist_tree.remove(elist.getKey());
								log.write("Eliminando lista " + elist.getName(), res!=4, true);
							}
							else if (action==2)
							{
								break;
							}
						}
					}
					else if (action==7)	// Mantener candidatos
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
						ConfigurationEntry& electionListEntry = configuration.getEntry("ElectionList");
						ElectionsListMethods elm;
						Tree electionslist_tree(electionListEntry.getDataFileName(), electionListEntry.getBlockSize(),
								&elm, false);

						// Elections
						ConfigurationEntry& electionEntry = configuration.getEntry("Election");
						ElectionMethods em;
						Tree election_tree(electionEntry.getDataFileName(), electionEntry.getBlockSize(), &em, false);

						option candidate_action[3];
						candidate_action[0].label = "Agregar candidato";
						candidate_action[1].label = "Eliminar candidato";
						candidate_action[2].label = "Volver";
						while(1)
						{//candidate_tree.print();
							action = Menu(candidate_action,3).ask();
							if (action==0)
							{
								// Check if candidate exists as voter
								int dni = atoi(Menu::raw_input("DNI").c_str());
								char day;
								char month;
								short year;

								enterValidDate(&year, &month, &day);

								Candidate cand (day, month, year, Menu::raw_input("Lista"), Menu::raw_input("Cargo"), dni);
								VariableRecord *record;
								Voter v(dni, "", "", "", "", std::vector<ElectionKey>());
								bool voterFound = hash_voter.getRecord(v.getKey(), &record);
								if (!voterFound)
								{
									cout << endl << "El candidato no pertenece al padron!" << endl;
								}
								else
								{
									// Check if Election list exists
									ElectionsList elist (cand.getListName(),cand.getDay(), cand.getMonth(), cand.getYear(), cand.getCharge());
									OpResult result = electionslist_tree.remove(elist.getKey());
									if (result == NotFound)
									{
										cout << endl << "La lista no existe!" << endl;
										cout << "Las posibles listas son: " << endl;
										electionslist_tree.print();
										cout << endl;
									}
									else
									{
										VariableRecord candkey_vr (cand.getKey(), cand.getKeySize());
										VariableRecord cand_vr (cand.getBytes(), cand.getSize());
										OpResult res = candidate_tree.insert(&candkey_vr, &cand_vr);
										log.write("Agregando candidato", res!=NotFound, true);
									}

								}

							}
							else if (action==1)
							{
								int dni = atoi(Menu::raw_input("DNI").c_str());
								char day;
								char month;
								short year;

								enterValidDate(&year, &month, &day);

								Candidate cand (day, month, year, Menu::raw_input("Nombre"), Menu::raw_input("Cargo"), dni);
								int res = candidate_tree.remove(cand.getKey());
								log.write("Eliminando candidato", res!=4, true);
							}
							else if (action==2)
							{
								break;
							}
						}

					}
					else if (action==8)
					{
//						admin_tree->print();
						option administrator_action[3];
						administrator_action[0].label = "Agregar administrador";
						administrator_action[1].label = "Eliminar administrador";
						administrator_action[2].label = "Volver";
						while(1)
						{
//							admin_tree->print();
							action = Menu(administrator_action,3).ask();
							if (action==0)
							{
								Administrator newadmin(Menu::raw_input("Usuario"), Menu::raw_input("Contraseña"));
								VariableRecord adminkey_vr (newadmin.getKey(), newadmin.getKeySize());
								VariableRecord admin_vr (newadmin.getBytes(), newadmin.getSize());
								int res = admin_tree.insert(&adminkey_vr, &admin_vr);
//								cout << "KEY: "<<newadmin.getKey() << endl;
//								cout << "KEY: "<<newadmin.getKeySize() << endl;
//								cout << "KEY: "<<newadmin.getBytes() << endl;
//								cout << "KEY: "<<newadmin.getBytes()+5 << endl;
//								cout << "KEY: "<<newadmin.getSize() << endl;
								log.write(string("Agregando administrador ").append(newadmin.getName()), res!=5, true);
							}
							else if (action==1)
							{
								Administrator remadmin(Menu::raw_input("Usuario"), "");
								cout << remadmin.getKey()<<endl;
								int res = admin_tree.remove(remadmin.getKey());
								log.write(string("Eliminando administrador ").append(remadmin.getName()), res!=4, true);
							}
							else if (action==2)
							{
								break;
							}
						}

					}
					else if (action==9)
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
								stringstream logStr;
								stringstream report;
								std::string district = Menu::raw_input("Distrito");
								DistrictCounts indexDistrict(district);
								ConfigurationEntry& disRprt_disCountentry = configuration.getEntry("DistrictCounts");
								DistrictCountsMethods districtCountsMethods;
								Tree index_districtCount (disRprt_disCountentry.getDataFileName(), disRprt_disCountentry.getBlockSize(), &districtCountsMethods, false);
								VariableRecord countIdRecord;
								if(! index_districtCount.get(indexDistrict.getKey(), &countIdRecord) )
								{
									report << "No election found in: " << indexDistrict.getDistrict() << endl;
									log.write(string("Reporte distrito invalido: ").append(indexDistrict.getDistrict()),false, true );//lista invalida
									break;
								}
								indexDistrict.setBytes(countIdRecord.getBytes());
								std::vector<CountId> countsId = indexDistrict.getCountIds();

								sort (countsId.begin(), countsId.end(), countIdCmp);

								CountId countId = countsId.at(0);//first count id
								int totalCountId = countsId.size();
								Count count(countId.getDay(), countId.getMonth(), countId.getYear(), countId.getCharge(), countId.getListName(), indexDistrict.getDistrict(), 0);
								ConfigurationEntry& disRprt_entry = configuration.getEntry("Count");
								CountMethods countMethods;
								Tree list_count(disRprt_entry.getDataFileName(), disRprt_entry.getBlockSize(), &countMethods, false);
								VariableRecord countRecord;
								std::cout << "Distrito: " << district << endl;
								logStr << string("Reportando distrito: ").append(district);
								log.operator <<(logStr.str());


								list_count.get(count.getKey(), &countRecord);
								count.setBytes(countRecord.getBytes());
								int listVotes= count.getQuantity();
								std::vector<string> listNames;
								listNames.push_back(count.getListName());
								for(int i=1;i<totalCountId;i++)
								{
									countId = countsId.at(i);
									Count nextCount(countId.getDay(), countId.getMonth(), countId.getYear(), countId.getCharge(), countId.getListName(), indexDistrict.getDistrict(), 0);
									list_count.get(nextCount.getKey(), &countRecord);
									nextCount.setBytes(countRecord.getBytes());

									if(( count.getDay() == nextCount.getDay() )&&( count.getMonth() == nextCount.getMonth() )&&( count.getYear() == nextCount.getYear() )&&( count.getCharge()== nextCount.getCharge()))
									{
										//same election
										if(nextCount.getQuantity() == listVotes )
										{
											listNames.push_back(nextCount.getListName());
										}
										else if(nextCount.getQuantity() > listVotes)
										{
											listNames.clear();
											listNames.push_back(nextCount.getListName());
											listVotes = nextCount.getQuantity();
										}
									}
									else
									{
										//change of election. print previous one
										report << "Ganador de Eleccion " << count.getCharge()<<", "<< (int)count.getDay()<<"-"<< (int)count.getMonth()<<"-"<< count.getYear() << endl;
										//log.write(string("Eleccion: ").append(indexDistrict.getDistrict()), true, true);
										for(int j=0;j< listNames.size();j++)
										{
											report << "--Lista: " << listNames.at(j) << ", votos: " << listVotes << endl;
										}
										listNames.clear();
										count.setBytes(nextCount.getBytes());//saving new election
										listVotes = count.getQuantity();
										listNames.push_back(count.getListName());
									}
								}
								//print last election
								report << "Ganador de Eleccion " << count.getCharge()<<", "<< (int)count.getDay()<<"-"<< (int)count.getMonth()<<"-"<< count.getYear() << endl;
								for(int j=0;j< listNames.size();j++)
								{
									report << "--Lista: " << listNames.at(j) << ", votos: " << listVotes << endl;
								}
								logStr << string("Finalizado Reporte por Distrito: ").append(indexDistrict.getDistrict());
								log.operator <<(logStr.str());
								std::cout << report.str();
								saveReport(report);
							}
							if(action == 1)
							{
								stringstream report;
								ConfigurationEntry& listRprt_candidateEntry = configuration.getEntry("Candidate");
								CandidateMethods cm;
								stringstream logStr;
								char day;
								char month;
								short year;

								enterValidDate(&year, &month, &day);

								std::string listName = Menu::raw_input("Lista");
								std::string charge = Menu::raw_input("Cargo");
								Tree candidate_tree (listRprt_candidateEntry.getDataFileName(), listRprt_candidateEntry.getBlockSize(), &cm, false);
								Candidate candidate(day, month, year, listName, charge, 0);
								VariableRecord candRecord;
								candidate_tree.get(candidate.getKey(), &candRecord);
								if(candRecord.getSize() == 0)
								{
									report << "Invalid list. List was not found." << endl;
									log.write(string("Reporte Lista invalida: ").append(listName),false, true );//lista invalida
									break;
								}
								candidate.setBytes(candRecord.getBytes());//got candidate of list

								if((day != candidate.getDay()) || (month != candidate.getMonth()) || (year != candidate.getYear()) || (strcmp(listName.c_str(), candidate.getListName().c_str()) !=0)|| (strcmp(charge.c_str(), candidate.getCharge().c_str()) !=0) )
								{
									report << "Invalid list. List was not found." << endl;
									log.write(string("Reporte Lista invalida: ").append(listName),false, true );//lista invalida
									break;
								}
								//get name from dni
								ConfigurationEntry& listRprt_voterEntry = configuration.getEntry("Voter");
								VoterMethods vm;
								VoterHashingFunction vhf;
								HashBlockFile hash_voter(listRprt_voterEntry.getDataFileName(), listRprt_voterEntry.getBlockSize(),&vm, &vhf,dataFiles.getVoterBlockAmount(), false);
								VariableRecord* voterRecord = NULL;
								Voter voter(0, "invalid", "invalid", "invalid", "invalid", std::vector<ElectionKey>());
								int dni = candidate.getDni();
								char keyDni[sizeof(int)];
								memcpy(keyDni, &dni, sizeof(int));
								hash_voter.getRecord(keyDni, &voterRecord);
								voter.setBytes(voterRecord->getBytes());
								if(voterRecord!= NULL){ delete voterRecord; }

								ConfigurationEntry& listRprt_countEntry = configuration.getEntry("Count");
								CountMethods countMethods;
								Tree list_count(listRprt_countEntry.getDataFileName(), listRprt_countEntry.getBlockSize(),&countMethods, false);
								VariableRecord countRecord;
								report << "Lista: " << candidate.getCharge() <<", "<< (int)candidate.getDay() <<"-"<< (int)candidate.getMonth() <<"-"<< candidate.getYear() <<", lista: "<< candidate.getListName() << endl;
								report << "Candidato: " << candidate.getCharge() << ", " << voter.getNames() << endl;

								stringstream strDay; strDay << (int)day;
								stringstream strMonth; strMonth << (int)month;
								stringstream strYear; strYear << year;
								logStr << string("Reportando lista ").append(listName+", "+ strDay.str() +" "+ strMonth.str() + " " + strYear.str()+ "-" + charge);
								log.operator <<(logStr.str());
								int totalCount = 0;
								char votes[sizeof(int)];
								int countVotes;

								Count count(candidate.getDay(), candidate.getMonth(), candidate.getYear(), candidate.getCharge(), candidate.getListName(),"", 0);
								list_count.get(count.getKey(),&countRecord );
								if(countRecord.getSize() == 0)
								{
									report << "No hay votos registrados para lista: " << count.getListName() << endl;
									log.write(string("Lista sin votos: ").append(candidate.getListName()), false, true);
									break;
								}

								Count nextCount(candidate.getDay(), candidate.getMonth(), candidate.getYear(), candidate.getCharge(), candidate.getListName(),"", 0);
								nextCount.setBytes(countRecord.getBytes());
								while(strcmp(count.getListName().c_str(), nextCount.getListName().c_str())==0 ){
									count.setBytes(nextCount.getBytes());
									std::string district = count.getDistrict();
									countVotes = count.getQuantity();
									report << "Distrito: " << district << endl;
									report << "Votos: " << countVotes << endl;
									memcpy(votes, &(countVotes), sizeof(int));
									totalCount+= countVotes;
									if(list_count.getNext(&countRecord) == NULL)
									{
										break;//last Record found
									}
									nextCount.setBytes(countRecord.getBytes());
								}
								char total[sizeof(int)];
								memcpy(total, &totalCount, sizeof(int));
								std::cout << "Votos Totales: " << totalCount << endl;
								strDay.clear(); strDay << (int)day;
								strMonth.clear(); strMonth << (int)month;
								strYear.clear(); strYear << year;
								logStr << string("Finalizado Reporte por Lista ").append(listName+", "+ strDay.str()+" "+strMonth.str()+" "+strYear.str()+"-"+charge);
								log.operator <<(logStr.str());
								std::cout << report.str();
								saveReport(report);
							}
							if(action == 2)
							{
								//report by election
								stringstream report;
								stringstream logStr;
								char day;
								char month;
								short year;

								enterValidDate(&year, &month, &day);

								std::string charge = Menu::raw_input("Cargo");
								ConfigurationEntry& elctnRprt_listEntry = configuration.getEntry("ElectionList");
								ElectionsListMethods electionsListMethods;
								Tree list_tree (elctnRprt_listEntry.getDataFileName(), elctnRprt_listEntry.getBlockSize(), &electionsListMethods, false);
								ElectionsList list("", day, month, year, charge);
								ElectionsList nextList("", day, month, year, charge);
								VariableRecord listRecord;

								stringstream strDay; strDay << (int)day;
								stringstream strMonth; strMonth << (int)month;
								stringstream strYear; strYear << year;

								list_tree.get(list.getKey(), &listRecord);
								if(listRecord.getSize() == 0)
								{//reached end of file
									report << "Invalid Election. Election was not found." << endl;

									log.write(string("Reporte Eleccion invalida: ").append(charge+", "+strDay.str() +"-"+ strMonth.str() +"-"+ strYear.str()),false, true );//lista invalida
									break;
								}
								list.setBytes(listRecord.getBytes());
								if((list.getDay() != nextList.getDay() )||( list.getMonth() != nextList.getMonth() )||( list.getYear() != nextList.getYear() )||( strcmp(list.getCharge().c_str(), nextList.getCharge().c_str()) != 0 ))
								{//got next election
									report << "Invalid Election. Election was not found." << endl;
									log.write(string("Reporte Eleccion invalida: ").append(charge+", "+strDay.str() +"-"+ strMonth.str() +"-"+ strYear.str()),false, true );//lista invalida
									break;
								}
								nextList.setBytes(list.getBytes());
								Count count(list.getDay(), list.getMonth(), list.getYear(), list.getCharge(), list.getName(),"", 0);
								ConfigurationEntry& elctnRprt_countEntry = configuration.getEntry("Count");
								CountMethods countMethods;
								Tree list_count(elctnRprt_countEntry.getDataFileName(), elctnRprt_countEntry.getBlockSize(), &countMethods, false);
								VariableRecord countRecord;
								Count nextCount(list.getDay(), list.getMonth(), list.getYear(), list.getCharge(), list.getName(),"", 0);

								report << "Eleccion: " << list.getCharge() <<", "<< day <<"-"<< month <<"-"<< year << endl;
								logStr << string("Reportando Eleccion: ").append(list.getCharge()+", "+strDay.str() +"-"+ strMonth.str() +"-"+ strYear.str());
								log.operator <<(logStr.str());
								char votos[sizeof(int)];
								list_count.get(count.getKey(), &countRecord);
								if(countRecord.getSize() == 0)
								{
									//No record, end of file
									report << "No hay votos registrados para eleccion: " << day << "-" << month << "-" << year << ", " << count.getCharge()<< endl;
									logStr << string("Eleccion sin votos: ").append(list.getCharge()+", "+strDay.str()+"-"+ strMonth.str() +"-"+ strYear.str());
									log.operator <<(logStr.str());
									break;
								}
								nextCount.setBytes(countRecord.getBytes());

								//first list name was not found in count tree. no votes
								report << "Nota: algunas listas pudieron no obtener voto alguno en esta eleccion." << endl
										<< "Las mismas no seran especificadas en el reporte." << endl;

								bool lastRecord = false;
								while((count.getDay() == nextCount.getDay() )&&( count.getMonth() == nextCount.getMonth() )&&( count.getYear() == nextCount.getYear() )&&( strcmp(count.getCharge().c_str(), nextCount.getCharge().c_str())== 0) )
								{
									int listVotes = 0;
									bool votedList = false;
									lastRecord = false;
									count.setBytes(nextCount.getBytes());

									while( strcmp(count.getListName().c_str(), nextCount.getListName().c_str()) == 0 )
									{
										//acumulate votes for one list in each district

										listVotes+= nextCount.getQuantity();
										if(! list_count.getNext(&countRecord) )
										{
											lastRecord = true;
											break;
										}
										nextCount.setBytes(countRecord.getBytes());

									}
									memcpy(votos, &listVotes, sizeof(int));
									std::cout << "Lista: " << count.getListName() << ", Votos: " << listVotes << endl;
									if(lastRecord)
									{
										break;//no more districts
									}
								}
								report << "Finalizado reporte Eleccion " << count.getCharge() <<", "<< day << "-" << month << "-" << year << endl;
								logStr << string("Finalizado reporte Eleccion ").append(list.getCharge()+", "+strDay.str() +"-"+ strMonth.str() +"-"+ strYear.str());
								log.operator <<(logStr.str());
								std::cout << report.str();
								saveReport(report);
							}//close option for election report
							else if (action==3)
							{
								break;
							}
						}
					}
					else if (action == 10)
					{
						updateCountVoteAmount(configuration);
					}
					else if(action == 11)
					{
						recoverReport();
					}
					else if (action == 12)
					{
						crackReport();
					}
					else if (action == 13)
					{
						break;
					}
				}
//				delete admin_tree;
				break;
		}
	}
	return 0;
}
