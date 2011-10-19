/*
 * Voting.cpp
 *
 *  Created on: 08/10/2011
 *      Author: alejandro
 */

#include "Voting.h"
#include "Menu.h"
#include "../Hash/HashBlockFile.h"
#include "../Entities/VoterMethods.h"
#include "../Entities/Election.h"
#include "../Entities/ElectionMethods.h"
#include "../Entities/ElectionsList.h"
#include "../Entities/ElectionsListMethods.h"
#include "../Hash/VoterHashingFunction.h"
#include "../BPlusTree/TreeBlockFile.h"
#include "../VariableBlocks/Constants.h"
#include "../Entities/Count.h"
#include "../Entities/CountMethods.h"
#include "../Entities/VoterMethods.h"
#include "../Indexes/DistrictCountsIndex.h"
#include "../Indexes/DistrictElectionsIndex.h"
#include "../Indexes/DistrictElections.h"
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

static const int MAX_LINE_SIZE = 400;

Voting::Voting(Configuration* configuration):config(configuration)
{
    this->voter = NULL;
}

bool Voting::login(int voterBlockAmount)
{
	ConfigurationEntry& voterEntry = this->config->getEntry("Voter");
    string voterFileName = voterEntry.getDataFileName();
    int voterBlockSize = voterEntry.getBlockSize();

    VoterMethods vm;
    VoterHashingFunction vhf;
    HashBlockFile voterFile(voterFileName, voterBlockSize, &vm, &vhf, voterBlockAmount, false);

    ifstream voterFileTxt;
    voterFileTxt.open(voterEntry.getLoadFileName().c_str());

    char line[MAX_LINE_SIZE];
    int i = 0;
    while(voterFileTxt.getline(line, MAX_LINE_SIZE))
    {
        string dni = strtok(line, ","); strtok(NULL, ",");
        string pass = strtok(NULL, ",");
        int intDni = atoi(dni.c_str());

        this->voter = new Voter(intDni, "invalid", "invalid", "invalid", "invalid", std::vector<ElectionKey>());

        VariableRecord* voterRecord = new VariableRecord();

        if(!voterFile.getRecord(this->voter->getKey(), &voterRecord))
        {
        	// no existe el votante en el arbol
        	continue;
        }

        this->voter->setBytes((char*) voterRecord->getBytes());

        if(strcmp(this->voter->getPassword().c_str(), pass.c_str()) != 0)
        {
            // loguear error
            continue;
        }

        delete voterRecord;

        cout << "Votante: " << i << endl;
        i++;
        this->vote();

        delete this->voter;
    }

    voterFileTxt.close();

    ConfigurationEntry& countEntry = this->config->getEntry("Count");
	string countFileName = countEntry.getDataFileName();
	int countBlockSize = countEntry.getBlockSize();
	CountMethods countMethods;
	Tree countTree(countFileName, countBlockSize, &countMethods, false);
	countTree.print();
    return true;
}

bool Voting::vote()
{
	// indice para obtener elecciones por distrito
	ConfigurationEntry& districtEntry = this->config->getEntry("DistrictElections");
	DistrictElectionsIndex districtElectionsIndex(districtEntry.getDataFileName(), districtEntry.getBlockSize(), false);
	VariableRecord districtRecord;
	bool electionsFound = districtElectionsIndex.getDistrictElections(this->voter->getDistrict(), &districtRecord);

	if(!electionsFound)
	{
		return false;
	}

	DistrictElections dElections;
	dElections.setBytes(districtRecord.getBytes());

	if(!dElections.hasElections())
	{
		// el distrito no tiene elecciones asignadas. loguear
		return false;
	}

	vector<ElectionId>& electionsId = dElections.getElectionIds();
	vector<Election> districtElection;

	ConfigurationEntry& electionsEntry = this->config->getEntry("Election");
	string electionFileName = electionsEntry.getDataFileName();
	int electionBlockSize = electionsEntry.getBlockSize();
	ElectionMethods electionMethods;
	Tree electionTree(electionFileName, electionBlockSize, &electionMethods, false);

	for(int i = 0; i < electionsId.size(); i++)
	{
		Election elec(electionsId.at(i).getDay(), electionsId.at(i).getMonth(), electionsId.at(i).getYear(), electionsId.at(i).getCharge());
		VariableRecord electionRecord;
		bool electionFounded = electionTree.get(elec.getKey(), &electionRecord);

		if(!electionFounded)
		{
			return false;
		}

		elec.setBytes(electionRecord.getBytes());

		districtElection.push_back(elec);
	}

	ConfigurationEntry& entry = this->config->getEntry("DistrictCounts");
	DistrictCountsIndex districtCountsIndex(entry.getDataFileName(), entry.getBlockSize(), true);

	ConfigurationEntry& countEntry = this->config->getEntry("Count");
	string countFileName = countEntry.getDataFileName();
	int countBlockSize = countEntry.getBlockSize();
	CountMethods countMethods;
	Tree countTree(countFileName, countBlockSize, &countMethods, false);

    for(unsigned int i = 0; i < districtElection.size(); i++)
    {
        vector<ElectionsList> electionsLists = this->getElectionsListsByElection(&(districtElection.at(i)));

        if(electionsLists.size() != 0)
        {
            srand(time(NULL));
            int size = electionsLists.size();
            int listIndex = rand() % size;

            ElectionsList list = electionsLists.at(listIndex);

            if(this->isInVoterElectionList(&(districtElection.at(i))))
            {
            	continue;
            }

			ElectionKey eKey = {districtElection.at(i).getYear(), districtElection.at(i).getMonth(),
					districtElection.at(i).getDay(), districtElection.at(i).getCharge()};
			this->voter->getElectionKeyList().push_back(eKey); // para que esto funque bien si no me equivoco getElectionKeyList tiene que devolver vector& no?

			Count c(districtElection.at(i).getDay(), districtElection.at(i).getMonth(), districtElection.at(i).getYear(),
					districtElection.at(i).getCharge(), list.getName(), this->voter->getDistrict(), 0);

			char key[c.getKeySize()];
			memcpy(key, c.getKey(), c.getKeySize());

			VariableRecord record;
			bool found = countTree.get(c.getKey(), &record);

			if(!found)
			{
				c.increaseQuantity();
				VariableRecord keyRecord(c.getKey(), c.getKeySize());
				record.setBytes(c.getBytes(), c.getSize());
				countTree.insert(&keyRecord, &record);
				districtCountsIndex.indexCount(c);
			}

			else
			{
				c.setBytes(record.getBytes());
				c.increaseQuantity();
				record.setBytes(c.getBytes(), c.getSize());
				countTree.update(key, &record);
			}
        }

        else return false;
    }
    return true;
}

vector<ElectionsList> Voting::getElectionsListsByElection(Election* e)
{
	ConfigurationEntry& electionsListEntry = this->config->getEntry("ElectionList");
	string electionsListFileName = electionsListEntry.getDataFileName();
	int electionsListBlockSize = electionsListEntry.getBlockSize();

	ElectionsListMethods elm;
	Tree electionsListTree(electionsListFileName, electionsListBlockSize, &elm, false);

	vector<ElectionsList> electionsListVector;

	VariableRecord record;
	ElectionsList electionList("", e->getDay(), e->getMonth(), e->getYear(), e->getCharge());

	electionsListTree.get(electionList.getKey(), &record);

	electionList.setBytes(record.getBytes());
	bool change = this->verifyElectionKeyChanges(electionList, e);

	while(!change)
	{
		electionsListVector.push_back(electionList);

		if (electionsListTree.getNext(&record) == NULL)
		{
			break;
		}

		if(record.getBytes() == NULL) break;

		electionList.setBytes(record.getBytes());

		change = this->verifyElectionKeyChanges(electionList, e);
	}

	return electionsListVector;
}

bool Voting::verifyElectionKeyChanges(ElectionsList& eList, Election* election)
{
	return (eList.getDay() != election->getDay()
				|| eList.getMonth() != election->getMonth()
				|| eList.getYear() != election->getYear()
				|| eList.getCharge() != election->getCharge());
}

/*vector<ElectionsList> Voting::getElectionsListsByElection(Election* e)
{
	vector<ElectionsList> electionsLists;
	ElectionsList eOne("Lista1", 1, 2, 2009, "Intendente");
	ElectionsList eTwo("Lista2", 3, 2, 2009, "Gobernador");
	ElectionsList eThree("Lista3", 1, 4, 2009, "Presidente");
	ElectionsList eFour("Lista4", 12, 6, 2009, "Senador");
	ElectionsList eFive("Lista5", 19, 7, 2009, "Diputado");

	ElectionsList elecs[] = {eOne, eTwo, eThree, eFour, eFive};
	vector<ElectionsList> retVec;
	for(int i = 0; i < 5; i++)
	{
		retVec.push_back(elecs[i]);
	}

	return retVec;
}*/

vector<Election> Voting::getElectionByDistrict(District* d)
{
	vector<string> districts;
	districts.push_back(d->getName());
	Election eOne(1, 2, 2009, "Intendente", districts);
	Election eTwo(3, 2, 2009, "Gobernador", districts);
	Election eThree(1, 4, 2009, "Presidente", districts);
	Election eFour(12, 6, 2009, "Senador", districts);
	Election eFive(19, 7, 2009, "Diputado", districts);

	Election elecs[] = {eOne, eTwo, eThree, eFour, eFive};
	vector<Election> retVec;
	for(int i = 0; i < 5; i++)
	{
		retVec.push_back(elecs[i]);
	}

	return retVec;
}

bool Voting::isInVoterElectionList(Election* e)
{
	for(unsigned int i = 0; i < this->voter->getElectionKeyList().size(); i++)
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
