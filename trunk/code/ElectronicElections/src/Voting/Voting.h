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

using namespace std;

class Voting {
private:
	LoadDataFiles* dataFiles;
	Voter* voter;
	bool isInVoterElectionList(Election* e);
	bool voteInElection(Election* e);
	bool isInVoterDistrict(Election* e);
public:
	Voting(LoadDataFiles* dataFiles);
	bool login();
	bool vote();
	virtual ~Voting();
};

#endif /* VOTING_H_ */
