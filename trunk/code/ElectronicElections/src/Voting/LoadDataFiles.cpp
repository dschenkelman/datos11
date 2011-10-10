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
	this->bSize = atoi(fields[1]);
	this->regsCount = atoi(fields[2]);
	this->regsSize = atoi(fields[3]);

	if(strcmp(fileType, "District") == 0)
	{
		this->treeDistrictFile = new Tree("DistrictTree", this->bSize, new DistrictMethods, true);
		this->readDistrictFile(fields[0]);
	}

	else if(strcmp(fileType, "Voter") == 0)
	{
		//hash types..calculate blockamount
		this->voterBlockSize = this->bSize * 4/5; //reserve 20% of free block
		this->voterBlockAmount = this->regsCount * this->regsSize /this->voterBlockSize;

		this->hashVoterFile = new HashBlockFile("VoterHash", this->bSize, new VoterMethods, new VoterHashingFunction, this->voterBlockAmount, true);
		this->readVoterFile(fields[0]);
	}

	else if(strcmp(fileType, "Charge") == 0)
	{
		this->chargeBlockAmount = 9;
		this->hashChargeFile = new HashBlockFile("ChargeHash", this->bSize, new ChargeMethods, new ChargeHashingFunction, this->chargeBlockAmount, true);
		this->readChargeFile(fields[0]);
	}
}

void LoadDataFiles::readDistrictFile(char* dataFileName)
{

}

void LoadDataFiles::readVoterFile(char* dataFileName)
{
	fstream dataFile;
	dataFile.open(dataFileName, ios::in |  ios::binary);
	std::string line;
	char* nombre;
	char* pass;
	char* domicilio;
	char* district;
	char* dni;
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
		this->hashVoterFile->loadRecord(voter->getKey(), record);
		delete voter;
		delete record;
	}
	dataFile.close();
}

void LoadDataFiles::readChargeFile(char* dataFileName)
{
	fstream dataFile;
	dataFile.open(dataFileName, ios::in |  ios::binary);
	std::string line;
	char* ppalCharge;
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
		this->hashChargeFile->loadRecord(charge->getKey(), record);
		delete charge;
		delete record;
	}
	dataFile.close();
}

LoadDataFiles::~LoadDataFiles()
{
	delete this->hashChargeFile;
	delete this->hashVoterFile;
	delete this->treeDistrictFile;
}
