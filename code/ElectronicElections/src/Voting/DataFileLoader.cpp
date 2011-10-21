/*
 * LoadDataFiles.cpp
 *
 *  Created on: 09/10/2011
 *      Author: gabriel
 */

#include "DataFileLoader.h"
#include <iostream>
#include "../Indexes/DistrictElectionsIndex.h"

using namespace std;

DataFileLoader::DataFileLoader(Configuration& config):configuration(config)
{
}

int DataFileLoader::getVoterBlockAmount()
{
	return this->voterBlockAmount;
}

void DataFileLoader::loadDistrictsFile()
{
    cout << "Generando archivo de distritos" << endl;
    DistrictMethods districtMethods;
    ConfigurationEntry entry = this->configuration.getEntry("District");
    Tree treeDistrictFile(entry.getDataFileName(), entry.getBlockSize(), &districtMethods, true);
    this->readDistrictFile(&treeDistrictFile, entry);
}

void DataFileLoader::loadElectionsFile()
{
    ConfigurationEntry & entry = this->configuration.getEntry("Election");
    cout << "Generando archivo de elecciones" << endl;
    cout << entry.getBlockSize() << endl;
    ElectionMethods em;
    Tree electionTree(entry.getDataFileName(), entry.getBlockSize(), &em, true);
    this->readElectionFile(&electionTree, entry);
}

void DataFileLoader::loadElectionListsFile()
{
    ConfigurationEntry & entry = this->configuration.getEntry("ElectionList");
    cout << "Generando archivo de listas" << endl;
    ElectionsListMethods elm;
    Tree listTree(entry.getDataFileName(), entry.getBlockSize(), &elm, true);
    this->readListFile(&listTree, entry);
}

void DataFileLoader::loadCountsFile()
{
    ConfigurationEntry & entry = this->configuration.getEntry("Count");
    cout << "Generando archivo de conteo" << endl;
    CountMethods cm;
    Tree(entry.getDataFileName(), entry.blockSize, &cm, true);
}

void DataFileLoader::loadCandidatesFile()
{
    ConfigurationEntry & entry = this->configuration.getEntry("Candidate");
    cout << "Generando archivo de candidato" << endl;
    CandidateMethods cm;
    Tree candidateTree(entry.getDataFileName(), entry.getBlockSize(), &cm, true);
    this->readCandidateFile(&candidateTree, entry);
}

void DataFileLoader::loadVotersFile()
{
    ConfigurationEntry & entry = this->configuration.getEntry("Voter");
    int efficientBSize = entry.getBlockSize() * 4 / 5;
    this->voterBlockAmount = entry.getRegisterCount() * entry.getRegisterSize() / efficientBSize;
    cout << "Generando archivo de votantes" << endl;
    VoterMethods vm;
    VoterHashingFunction vhf;
    HashBlockFile hashVoterFile(entry.getDataFileName(), entry.getBlockSize(), &vm, &vhf, this->voterBlockAmount, true);
    this->readVoterFile(&hashVoterFile, entry);
}

void DataFileLoader::loadFiles()
{
	this->loadDistrictsFile();
	this->loadElectionsFile();
	this->loadElectionListsFile();
    this->loadCountsFile();
    this->loadCandidatesFile();
    this->loadVotersFile();

    ConfigurationEntry& entry = this->configuration.getEntry("Charge");
	cout << "Generando archivo de cargos" << endl;
    ChargeMethods cm;
    ChargeHashingFunction chf;
    int efficientBSize = entry.getBlockSize() * 4 / 5;
    this->chargeBlockAmount = entry.getRegisterCount() * entry.getRegisterSize() / efficientBSize +1;
    HashBlockFile hashChargeFile(entry.getDataFileName(), entry.getBlockSize(), &cm, &chf, this->chargeBlockAmount, true);
	this->readChargeFile(&hashChargeFile, entry);
}

bool DataFileLoader::canOpenAdminFile()
{
	fstream adminFile;
	ConfigurationEntry& entry = this->configuration.getEntry("Administrator");
	bool valid = false;
	adminFile.open(entry.getDataFileName().c_str(), ios::in | ios::binary | ios::out);
	if( adminFile.is_open())
	{
		valid = true;
		adminFile.close();
	}
	return valid;
}

Tree* DataFileLoader::createAdminFile()
{
	cout << "Generando archivo de administrador" << endl;
	ConfigurationEntry& entry = this->configuration.getEntry("Administrator");
	AdministratorMethods am;
	return new Tree(entry.getDataFileName().c_str(), entry.getBlockSize(), new AdministratorMethods, true);
}
Tree* DataFileLoader::getAdminFile()
{
	ConfigurationEntry& entry = this->configuration.getEntry("Administrator");
	AdministratorMethods am;
	return new Tree(entry.getDataFileName().c_str(), entry.getBlockSize(), new AdministratorMethods, true);
}

void DataFileLoader::readDistrictFile(Tree* treeDistrictFile, ConfigurationEntry& entry)
{
	fstream dataFile;
	dataFile.open(entry.getLoadFileName().c_str(), ios::in |  ios::binary);
	std::string line;
	while ( getline(dataFile,line) )
	{
		//list.clear();
		char* districtName = strtok((char*)line.c_str(), ",");

		District d(districtName);

//		District d(string(districtName));
		VariableRecord record (d.getBytes(), d.getSize());
		int res = treeDistrictFile->insert(&record, &record);
		Log().write(string("Agregando distrito ").append(d.getName()), res!=5, true);
	}
	dataFile.close();
}

void DataFileLoader::readCandidateFile(Tree* treeCandidateFile, ConfigurationEntry & entry)
{
	fstream dataFile;
	dataFile.open(entry.getLoadFileName().c_str(), ios::in |  ios::binary);
	std::string line;
	char* day;
	char* month;
	char* year;
	char* cargo;
	char* listName;
	char* dniVoter;
	while ( getline(dataFile,line) )
	{
		listName = strtok((char*)line.c_str(), ",");
		day = strtok(NULL, ",");
		month = strtok(NULL, ",");
		year = strtok(NULL, ",");
		dniVoter = strtok(NULL, ",");
		cargo = strtok(NULL, ",");
		short yearNumber = atoi(year);

		Candidate cand ((char)atoi(day), (char)atoi(month), yearNumber, string(listName), string(cargo), atoi(dniVoter));
		VariableRecord candkey_vr (cand.getKey(), cand.getKeySize());
		VariableRecord cand_vr (cand.getBytes(), cand.getSize());
		int res = treeCandidateFile->insert(&candkey_vr, &cand_vr);
		Log().write(string("Agregando candidato ").append(dniVoter), res!=5, true);
	}
	dataFile.close();
}

void DataFileLoader::readElectionFile(Tree* treeElectionFile, ConfigurationEntry& entry)
{
	ConfigurationEntry& districtElectionsEntry = this->configuration.getEntry("DistrictElections");
	DistrictElectionsIndex indexFile(districtElectionsEntry.getDataFileName(),
			districtElectionsEntry.getBlockSize(), true);

	fstream dataFile;
	dataFile.open(entry.getLoadFileName().c_str(), ios::in |  ios::binary);
	std::string line;
	char* month;
	char* year;
	char* cargo;
	char* firstDistrict;
	char* secondDistricts;
	while ( getline(dataFile,line) )
	{
		std::vector<std::string> distlist;
		char* day = strtok((char*)line.c_str(), ",");
		month = strtok(NULL, ",");
		year = strtok(NULL, ",");
		cargo = strtok(NULL, ",");
		firstDistrict = strtok(NULL, ",");
		distlist.push_back(string(firstDistrict));


		short yearNumber = atoi(year);
		cout << day <<"-"<< month<<"-"<<yearNumber<<"-"<<cargo<<endl;
		while((secondDistricts = strtok(NULL, ",")) != NULL)
		{
			distlist.push_back(secondDistricts);
		}
		Election election((char)atoi(day), (char)atoi(month), yearNumber, string(cargo), distlist);

		VariableRecord* record = new VariableRecord();
		record->setBytes(election.getBytes(), election.getSize());
		VariableRecord* key = new VariableRecord(election.getKey(), election.getKeySize());
		cout << treeElectionFile->insert(key, record)<<endl;

		indexFile.indexElection(election);
		delete record;
		delete key;
	}
	dataFile.close();
}

void DataFileLoader::readListFile(Tree* treeListFile, ConfigurationEntry & entry)
{
	fstream dataFile;
	dataFile.open(entry.getLoadFileName().c_str(), ios::in |  ios::binary);
	std::string line;
	char* month;
	char* year;
	char* cargo;
	char* listName;
	char* day;
	int i = 0;
	while ( getline(dataFile,line) )
	{
		day = strtok((char*)line.c_str(), ",");
		month = strtok(NULL, ",");
		year = strtok(NULL, ",");
		cargo = strtok(NULL, ",");
		listName = strtok(NULL, ",");
		short yearNumber = atoi(year);
		cout << ++i << endl;
		ElectionsList elist (string(listName), (char)atoi(day), (char)atoi(month), yearNumber, string(cargo));
		VariableRecord elistkey_vr (elist.getKey(), elist.getKeySize());
		VariableRecord elist_vr (elist.getBytes(), elist.getSize());
		int res = treeListFile->insert(&elistkey_vr, &elist_vr);
		Log().write(string("Agregando lista ").append(listName), res!=5, true);
	}
	dataFile.close();
}

void DataFileLoader::readVoterFile(HashBlockFile* hashVoterFile, ConfigurationEntry & entry)
{
	fstream dataFile;
	dataFile.open(entry.getLoadFileName().c_str(), ios::in);
	std::string line;
	char* nombre;
	char* pass;
	char* domicilio;
	char* district;

	int i = 0;

	while (getline(dataFile,line) )
	{
		char* dni = strtok((char*)line.c_str(), ",");
		nombre = strtok(NULL, ",");
		pass = strtok(NULL, ",");
		domicilio = strtok(NULL, ",");
		district = strtok(NULL, ",");
		Voter* voter = new Voter(atoi(dni), string(nombre), string(pass), string(domicilio), string(district));
		VariableRecord* record = new VariableRecord();
		record->setBytes(voter->getBytes(), voter->getSize());
		hashVoterFile->insertRecord(voter->getKey(), record);
		delete voter;
		delete record;
	}
	dataFile.close();
}

void DataFileLoader::readChargeFile(HashBlockFile* hashChargeFile, ConfigurationEntry & entry)
{
	fstream dataFile;
	dataFile.open(entry.getLoadFileName().c_str(), ios::in |  ios::binary);
	std::string line;
	std::vector<std::string> list;
	char* secondCharges;
	while ( getline(dataFile,line) )
	{
		list.clear();
		char* ppalCharge = strtok((char*)line.c_str(), ",");

		while((secondCharges = strtok(NULL, ",")) != NULL)
		{
			list.push_back(string(secondCharges));
		}
		Charge* charge = new Charge(string(ppalCharge), list);
		VariableRecord* record = new VariableRecord();
		record->setBytes(charge->getBytes(), charge->getSize());
		hashChargeFile->loadRecord(charge->getKey(), record);
		delete charge;
		delete record;
	}
	dataFile.close();
}

int DataFileLoader::getChargeBlockAmount()
{
	return this->chargeBlockAmount;
}

DataFileLoader::~DataFileLoader()
{
}
