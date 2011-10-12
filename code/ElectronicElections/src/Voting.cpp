/*
 * Voting.cpp
 *
 *  Created on: 08/10/2011
 *      Author: alejandro
 */

#include "Voting.h"
#include "Voting/Menu.h"
#include "Hash/HashBlockFile.h"
#include "Entities/VoterMethods.h"
#include "Entities/Election.h"
#include "Entities/ElectionMethods.h"
#include "Entities/ElectionsList.h"
#include "Entities/ElectionsListMethods.h"
#include "Hash/VoterHashingFunction.h"
#include "BPlusTree/TreeBlockFile.h"
#include "VariableBlocks/Constants.h"
#include <vector>
#include <cstring>
#include <iostream>

Voting::Voting(LoadDataFiles* dataFiles)
{
	this->dataFiles = dataFiles;
	this->voter = NULL;
}

bool Voting::login()
{
	string voterFileName = this->dataFiles->getVoterFileName();
	int voterBlockSize = this->dataFiles->getVoterBlockSize();
	int voterBlockAmount = this->dataFiles->getVoterBlockAmount();

	HashBlockFile voterFile(voterFileName, voterBlockSize, new VoterMethods, new VoterHashingFunction, voterBlockAmount, true);

	//string dni = Menu::raw_input("DNI");
	//string pass = Menu::raw_input("Password");

	string dni = "35094006";

	VariableRecord* voterRecord = new VariableRecord();
	if(!voterFile.getRecord(dni.c_str(), &voterRecord))
	{
		return false;
	}

	this->voter = new Voter(0, "invalid", "invalid", "invalid", "invalid", std::vector<ElectionKey>());
	this->voter->setBytes((char*) voterRecord->getBytes());

	delete voterRecord;

	/*if(strcmp(pass.c_str(), this->voter->getPassword().c_str()) != 0)
	{
		return false;
	}*/

	return true;
}

bool Voting::vote()
{
	/*string electionTreeName = this->dataFiles->getElectionFileName();
	int electionBlockSize = this->dataFiles->getElectionBlockSize();
	TreeBlockFile electionTree(electionTreeName, electionBlockSize, new ElectionMethods, true);

	while(!electionTree.isAtEOF())
	{
		Election e(1, 1, 1, "invalid", std::vector<string>());
		e.setBytes((char*) electionTree.getCurrentBlock()->getBytes());

		if(this->isInVoterElectionList(&e))
		{
			//
		}

		if(!this->isInVoterDistrict(&e))
		{

		}

		this->voteInElection(&e);
		// avanzar en el arbol
	}*/

	return true;
}

bool Voting::voteInElection(Election* e)
{
	//random para ver a quien vota
	return true;
}

bool Voting::isInVoterDistrict(Election* e)
{
	for(int i = 0; i < e->getDistrictList().size(); i++)
	{
		if(e->getDistrictList().at(i) == this->voter->getDistrict())
		{
			return true;
		}
	}

	return false;
}

bool Voting::isInVoterElectionList(Election* e)
{
	for(int i = 0; i < this->voter->getElectionKeyList().size(); i++)
	{
		short year = this->voter->getElectionKeyList().at(i).year;
		char month = this->voter->getElectionKeyList().at(i).month;
		char day = this->voter->getElectionKeyList().at(i).day;
		string charge = this->voter->getElectionKeyList().at(i).charge;

		int size = sizeof(short) + sizeof(char) * 2 + charge.size() + 1 + Constants::FIELD_HEADER_SIZE;

		char key[size]; int i = 0;

		memcpy(key+i, &(year), sizeof(short)); i += sizeof(short);
		memcpy(key+i, &(month), sizeof(char)); i += sizeof(char);
		memcpy(key+i, &(day), sizeof(char)); i += sizeof(char);

		char len = charge.size() + 1;
		memcpy(key+i, &len, sizeof(char)); i += Constants::FIELD_HEADER_SIZE;
		memcpy(key+i, charge.c_str(), len);

		if(strcmp(e->getKey(), key) == 0)
		{
			return true;
		}
	}

	return false;
}

Voting::~Voting()
{
}
