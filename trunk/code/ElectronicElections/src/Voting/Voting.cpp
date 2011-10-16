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
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

static const int MAX_LINE_SIZE = 400;

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

    ifstream voterFileTxt;
    voterFileTxt.open("padron.txt");

    char line[MAX_LINE_SIZE];
    while(voterFileTxt.getline(line, MAX_LINE_SIZE))
    {
        string dni = strtok(line, ","); strtok(NULL, ",");
        string pass = strtok(NULL, ",");
        int intDni = atoi(dni.c_str());

        this->voter = new Voter(intDni, "invalid", "invalid", "invalid", "invalid", std::vector<ElectionKey>());

        VariableRecord* voterRecord = new VariableRecord();

        if(!voterFile.getRecord(this->voter->getKey(), &voterRecord))
        {
        	return false;
        }

        cout << "Votante encontrado: " << this->voter->getNames();

        this->voter->setBytes((char*) voterRecord->getBytes());

        if(strcmp(this->voter->getPassword().c_str(), pass.c_str()) != 0)
        {
            // loguear error
            continue;
        }

        delete voterRecord;

        this->vote();

        delete this->voter;
    }

    voterFileTxt.close();
    return true;
}

bool Voting::vote()
{
	// indice para obtener elecciones por distrito
	District d(this->voter->getDistrict());
    vector<Election> districtElection = this->getElectionByDistrict(&d);

    for(int i = 0; i < districtElection.size(); i++)
    {
        /*string electionsListFileName = this->dataFiles->getElectionListFileName();
        int electionsListBlockSize = this->dataFiles->getElectionListBlockSize();
        Tree electionsListTree(electionsListFileName, electionsListBlockSize, &ElectionsListMethods(), true);*/

        // acá necesito las listas que tengan a esta eleccion como parte de su identificador.. indice?
        vector<ElectionsList> electionsLists = this->getElectionsListsByElection(&(districtElection.at(i)));

        if(electionsLists.size() != 0)
        {
            srand(time(NULL));
            int listIndex = rand() % (electionsLists.size());

            ElectionsList list = electionsLists.at(listIndex);

            if(this->isInVoterElectionList(&(districtElection.at(i))))
            {
            	continue;
            }

			ElectionKey eKey = {districtElection.at(i).getYear(), districtElection.at(i).getMonth(),
					districtElection.at(i).getDay(), districtElection.at(i).getCharge()};
			this->voter->getElectionKeyList().push_back(eKey); // para que esto funque bien si no me equivoco getElectionKeyList tiene que devolver vector& no?

/*			string countFileName = this->dataFiles->getCountFileName();
			int countBlockSize = this->dataFiles->getCountBlockSize();
			Tree countTree(countFileName, countBlockSize, &CountMethods(), true);

			Count c(districtElection.at(i).getDay(), districtElection.at(i).getMonth(), districtElection.at(i).getYear(),
					districtElection.at(i).getCharge(), list.getName(), this->voter->getDistrict(), 0);

			char key[c.getKeySize()];
			memcpy(key, c.getKey(), c.getKeySize());

			VariableRecord record;
			countTree.get(c.getKey(), &record);
			c.setBytes(record.getBytes());

			c.increaseQuantity();
			record.setBytes(c.getBytes(), c.getSize());

			countTree.update(key, &record);*/
        }

        else return false;
    }
    return true;
}

/*vector<ElectionsList> Voting::getElectionsListsByElection(Election* e)
{
	string electionsListFileName = this->dataFiles->getElectionListFileName();
	int electionsListBlockSize = this->dataFiles->getElectionListBlockSize();

	ElectionsListMethods elm;
	Tree electionsListTree(electionsListFileName, electionsListBlockSize, &elm, true);

	vector<ElectionsList> electionsListVector;

	VariableRecord record;
	electionsListTree.returnFirst(&record);

	while(record != NULL)
	{
		ElectionsList eList("invalid", 0, 0, 0, "invalid");
		eList.setBytes(record.getBytes());
		int compareResult = elm.compare(eList.getKey(), e->getBytes(), e->getSize());

		if(compareResult == 0)
		{
			electionsListVector.push_back(eList);
		}

		record = electionsListTree.getNext(&record);
	}

	return electionsListVector;
}*/

vector<ElectionsList> Voting::getElectionsListsByElection(Election* e)
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
}

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
