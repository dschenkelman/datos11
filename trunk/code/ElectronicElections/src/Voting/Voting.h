/*
 * Voting.h
 *
 *  Created on: 08/10/2011
 *      Author: alejandro
 */

#ifndef VOTING_H_
#define VOTING_H_

#include <string>
#include "LoadDataFiles.h"
#include "../Entities/Voter.h"
#include "../Entities/Election.h"
#include "../Entities/District.h"
#include "../Entities/ElectionsList.h"

using namespace std;

class Voting {
private:
	LoadDataFiles* dataFiles;
	Voter* voter;
	bool isInVoterElectionList(Election* e);
	vector<Election> getElectionByDistrict(District* d);
	vector<ElectionsList> getElectionsListsByElection(Election* e);
public:
	Voting(LoadDataFiles* dataFiles);
	bool login();
	bool vote();
	virtual ~Voting();
};

#endif /* VOTING_H_ */
