/*
 * LoadDataFiles.cpp
 *
 *  Created on: 09/10/2011
 *      Author: gabriel
 */

#include "LoadDataFiles.h"
#include <iostream>
using namespace std;

LoadDataFiles::LoadDataFiles(std::string configFileName)
{
	this->configName = configFileName;
	this->chargeFileName = "ChargeHash.dat";
	this->voterFileName = "VoterHash.dat";
	this->districtFileName = "District.dat";
	this->electionFileName = "Election.dat";
	this->candidateFileName = "Candidate.dat";
	this->electionListFileName = "ElectionList.dat";
	this->countFileName = "Count.dat";
	this->administratorFileName = "Administrator.dat";
	this->administratorBlockSize = 512;
}

void LoadDataFiles::readConfigFile()
{
	this->configFile.open(this->configName.c_str(), ios::binary | ios::in );
	std::string line;
	char* fields[4];
	while ( getline(this->configFile,line) )
	{
		char *fileType = strdup(line.c_str());
		strtok(fileType, ",");
		for(int i = 0; i<4; i++)
		{
			fields[i] = strtok(NULL, ",");
		}
		this->createFileType(fileType, fields);
	}
	this->configFile.close();
}

void LoadDataFiles::createFileType(char* fileType, char** fields)
{
	int regsCount = atoi(fields[2]);
	int regsSize = atoi(fields[3]);
	if(strcmp(fileType, "District") == 0)
	{
		this->districtBlockSize = atoi(fields[1]);
		cout << "Generando archivo de distritos" << endl;
		Tree* treeDistrictFile = new Tree(this->districtFileName, this->districtBlockSize, new DistrictMethods, true);
		this->readDistrictFile(treeDistrictFile, fields[0]);
		delete treeDistrictFile;
	}
	else if(strcmp(fileType, "Election") == 0)
	{
		this->electionBlockSize = atoi(fields[1]);
		//create it empty..
		cout << "Generando archivo de elecciones" << endl;
		Tree(this->electionFileName, this->electionBlockSize, NULL, true);
	}
	else if(strcmp(fileType, "ElectionList") == 0)
	{
		this->electionListBlockSize = atoi(fields[1]);
		//create it empty..
		cout << "Generando archivo de listas" << endl;
		Tree(this->electionListFileName, this->electionListBlockSize, NULL, true);
	}
	else if(strcmp(fileType, "Count") == 0)
	{
		this->countBlockSize = atoi(fields[1]);
		//create it empty..
		cout << "Generando archivo de conteo" << endl;
		Tree(this->countFileName, this->countBlockSize, NULL, true);
	}
	else if(strcmp(fileType, "Candidate") == 0)
	{
		this->candidateBlockSize = atoi(fields[1]);
		//create it empty..
		cout << "Generando archivo de candidato" << endl;
		Tree(this->candidateFileName, this->candidateBlockSize, NULL, true);
	}
	else if(strcmp(fileType, "Voter") == 0)
	{
		this->voterBlockSize = atoi(fields[1]);
		//hash type..calculate blockamount
		int efficientBSize = this->voterBlockSize * 4/5; //reserve 20% of free block
		this->voterBlockAmount = regsCount * regsSize /efficientBSize;
		cout << "Generando archivo de votantes" << endl;
		HashBlockFile* hashVoterFile = new HashBlockFile(this->voterFileName, this->voterBlockSize, new VoterMethods, new VoterHashingFunction, this->voterBlockAmount, true);
		this->readVoterFile(hashVoterFile, fields[0]);
		delete hashVoterFile;
	}
	else if(strcmp(fileType, "Charge") == 0)
	{
		this->chargeBlockAmount = 9;//Hardcode as there are 6 charges only!
		this->chargeBlockSize = atoi(fields[1]);
		cout << "Generando archivo de cargos" << endl;
		HashBlockFile* hashChargeFile = new HashBlockFile(this->chargeFileName, this->chargeBlockSize, new ChargeMethods, new ChargeHashingFunction, this->chargeBlockAmount, true);
		this->readChargeFile(hashChargeFile, fields[0]);
		delete hashChargeFile;
	}
}

bool LoadDataFiles::canOpenAdminFile()
{
	fstream adminFile;
	bool valid = false;
	adminFile.open(this->administratorFileName.c_str(), ios::in | ios::binary | ios::out);
	if( adminFile.is_open())
	{
		valid = true;
		adminFile.close();
	}
	return valid;
}
Tree LoadDataFiles::createAdminFile()
{
	cout << "Generando archivo de administrador" << endl;
	Tree treeAdministratorFile = Tree(this->administratorFileName, this->administratorBlockSize, new AdministratorMethods, true);
	return treeAdministratorFile;
}
Tree LoadDataFiles::getAdminFile()
{
	Tree treeAdministratorFile = Tree(this->administratorFileName, this->administratorBlockSize, new AdministratorMethods, false);
	return treeAdministratorFile;
}

void LoadDataFiles::readDistrictFile(Tree* treeDistrictFile, char* dataFileName)
{
	fstream dataFile;
	dataFile.open(dataFileName, ios::in |  ios::binary);
	std::string line;
	//std::vector<std::string> list;
	//char* secondCharges;
	while ( getline(dataFile,line) )
	{
		//list.clear();
		char* districtName = strdup(line.c_str());
		strtok(districtName, ",");

		//POSSIBLE INTERNAL DISTRICT LIST ON PRINCIPAL DISTRICT
		/*while((secondCharges = strtok(NULL, ",")) != NULL)
		{
			list.push_back(string(secondCharges));
		}*/

		District* district = new District(string(districtName));
		VariableRecord* record = new VariableRecord();
		record->setBytes(district->getBytes(), district->getSize());
		//treeDistrictFile->loadRecord(district->getKey(), record);
		delete district;
		delete record;
	}
	dataFile.close();
}

void LoadDataFiles::readVoterFile(HashBlockFile* hashVoterFile, char* dataFileName)
{
	fstream dataFile;
	dataFile.open(dataFileName, ios::in |  ios::binary);
	std::string line;
	char* nombre;
	char* pass;
	char* domicilio;
	char* district;
	std::vector<ElectionKey> list;
	ElectionKey electOne;
	char* electionField[4];

	while ( getline(dataFile,line) )
	{
		list.clear();
		char* dni = strdup(line.c_str());
		strtok(dni, ",");
		nombre = strtok(NULL, ",");
		pass = strtok(NULL, ",");
		domicilio = strtok(NULL, ",");
		district = strtok(NULL, ",");
		int i = 0;
		while((electionField[i] = strtok(NULL, ",")) != NULL)
		{
			if(i == 3)
			{
				electOne.year = atoi(electionField[0]);
				electOne.month = atoi(electionField[1]);
				electOne.day = atoi(electionField[2]);
				electOne.charge = string(electionField[3]);
				list.push_back(electOne);
			}
			i++;
			if(i > 3) i = 0;
		}
		Voter* voter = new Voter(atoi(dni), string(nombre), string(pass), string(domicilio), string(district), list);
		VariableRecord* record = new VariableRecord();
		record->setBytes(voter->getBytes(), voter->getSize());
		hashVoterFile->loadRecord(voter->getKey(), record);
		delete voter;
		delete record;
	}
	dataFile.close();
}

void LoadDataFiles::readChargeFile(HashBlockFile* hashChargeFile, char* dataFileName)
{
	fstream dataFile;
	dataFile.open(dataFileName, ios::in |  ios::binary);
	std::string line;
	std::vector<std::string> list;
	char* secondCharges;
	while ( getline(dataFile,line) )
	{
		list.clear();
		char* ppalCharge = strdup(line.c_str());
		strtok(ppalCharge, ",");

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

string LoadDataFiles::getChargeFileName(){
	return this->chargeFileName;
}
string LoadDataFiles::getVoterFileName(){
	return this->voterFileName;
}
string LoadDataFiles::getDistrictFileName(){
	return this->districtFileName;
}
string LoadDataFiles::getElectionFileName(){
	return this->electionFileName;
}
string LoadDataFiles::getCandidateFileName(){
	return this->candidateFileName;
}
string LoadDataFiles::getElectionListFileName(){
	return this->electionListFileName;
}
string LoadDataFiles::getCountFileName(){
	return this->countFileName;
}
string LoadDataFiles::getAdminFileName(){
	return this->administratorFileName;
}

int LoadDataFiles::getDistrictBlockSize(){
	return this->districtBlockSize;
}
int LoadDataFiles::getElectionBlockSize(){
	return this->electionBlockSize;
}
int LoadDataFiles::getElectionListBlockSize(){
	return this->electionListBlockSize;
}
int LoadDataFiles::getCountBlockSize(){
	return this->countBlockSize;
}
int LoadDataFiles::getCandidateBlockSize(){
	return this->candidateBlockSize;
}
int LoadDataFiles::getAdminBlockSize(){
	return this->administratorBlockSize;
}
int LoadDataFiles::getVoterBlockSize(){
	return this->voterBlockSize;
}
int LoadDataFiles::getVoterBlockAmount(){
	return this->voterBlockAmount;
}
int LoadDataFiles::getChargeBlockSize(){
	return this->chargeBlockSize;
}
int LoadDataFiles::getChargeBlockAmount(){
	return this->chargeBlockAmount;
}

LoadDataFiles::~LoadDataFiles()
{
}
