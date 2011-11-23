/*
 * Voting.h
 *
 *  Created on: 08/10/2011
 *      Author: alejandro
 */

#ifndef VOTING_H_
#define VOTING_H_

#include <string>
#include "DataFileLoader.h"
#include "../Entities/Voter.h"
#include "../Entities/Election.h"
#include "../Entities/District.h"
#include "../Entities/ElectionsList.h"
#include "Configuration.h"
#include "ConfigurationEntry.h"
#include "../Indexes/DistrictCountsIndex.h"

#include "../RSA/KeyManager.h"
#include "../RSA/RSACipher.h"

using namespace std;

class Voting {
private:
	DataFileLoader* dataFiles;
	Voter* voter;
	bool isInVoterElectionList(Election* e);
	bool verifyElectionKeyChanges(ElectionsList& eList, Election* election);
	vector<Election> getElectionByDistrict(District* d);
	vector<ElectionsList> getElectionsListsByElection(Election* e);
	Configuration* config;
	Log* log;
public:
	Voting(Configuration* config);
	bool login(int voterBlockAmount);
	bool vote(DistrictCountsIndex& districtCountsIndex);
	virtual ~Voting();
};

#endif /* VOTING_H_ */
