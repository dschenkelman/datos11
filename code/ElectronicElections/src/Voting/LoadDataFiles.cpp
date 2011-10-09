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
			fields[i] = strtok(NULL, ",\n");
		}
		this->createFileType(fileType, fields);
	}
	this->configFile.close();
}

void LoadDataFiles::createFileType(char* fileType, char** fields)
{
	int bSize = atoi(fields[1]);
	int regsCount = atoi(fields[2]);
	long regsSize = atoi(fields[3]);
	if(strcmp(fileType, "District") == 0)
		this->treeDistrictFile = new Tree(string(fields[0]), bSize, new DistrictMethods, true);

	//hash types..calculate blockamount
	int eficientBSize = bSize * 4/5; //reserve 20% of free block
	long blockAmount = regsCount* regsSize /eficientBSize;
	if(strcmp(fileType, "Voter") == 0)
		this->hashVoterFile = new HashBlockFile(string(fields[0]), bSize, new VoterMethods, new VoterHashingFunction, blockAmount, true);
	if(strcmp(fileType, "Charge") == 0)
		this->hashChargeFile = new HashBlockFile(string(fields[0]), bSize, new ChargeMethods, new ChargeHashingFunction, blockAmount, true);
}

void LoadDataFiles::readDistrictFile()
{

}

void LoadDataFiles::readVoterFile()
{

}

void LoadDataFiles::readChargeFile()
{

}

LoadDataFiles::~LoadDataFiles()
{
	delete this->hashChargeFile;
	delete this->hashVoterFile;
	delete this->treeDistrictFile;
}
