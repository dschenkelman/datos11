/*
 * LoadDataFiles.h
 *
 *  Created on: 09/10/2011
 *      Author: gabriel
 */

#ifndef DATAFILELOADER_H_
#define DATAFILELOADER_H_
#include <fstream>

#include "../Hash/HashBlockFile.h"
#include "../BPlusTree/Tree.h"

#include "../Entities/Voter.h"
#include "../Hash/VoterHashingFunction.h"
#include "../Entities/VoterMethods.h"

#include "../Entities/DistrictMethods.h"
#include "../Entities/AdministratorMethods.h"
#include "../Entities/Administrator.h"

#include "../Entities/Charge.h"
#include "../Hash/ChargeHashingFunction.h"
#include "../Entities/ChargeMethods.h"

#include "../Entities/Candidate.h"
#include "../Entities/ElectionsList.h"
#include "../Entities/ElectionsListMethods.h"
#include "../Entities/CandidateMethods.h"
#include "../Entities/CountMethods.h"
#include "../Entities/Election.h"
#include "../Entities/ElectionMethods.h"

#include "Log.h"
#include "Configuration.h"


class DataFileLoader
{
    std::string districtFileName;
    std::string electionFileName;
    std::string electionListFileName;
    std::string countFileName;
    std::string candidateFileName;
    std::string administratorFileName;
    std::string voterFileName;
    int voterBlockAmount;
    std::string chargeFileName;
    int chargeBlockAmount;
    Configuration configuration;
    void readElectionFile(Tree *treeElectionFile, ConfigurationEntry& entry);
    void readListFile(Tree *treeListFile, ConfigurationEntry & entry);
    void readDistrictFile(Tree *treeDistrictFile, ConfigurationEntry & entry);
    void readCandidateFile(Tree *treeCandidateFile, ConfigurationEntry & entry);
    void readVoterFile(HashBlockFile *hashVoterFile, ConfigurationEntry & entry);
    void readChargeFile(HashBlockFile *hashChargeFile, ConfigurationEntry & entry);
    void loadDistrictsFile();
    void loadElectionsFile();
    void loadElectionListsFile();
    void loadCountsFile();
    void loadCandidatesFile();
    void loadVotersFile();
    void loadChargesFile();
public:
	DataFileLoader(Configuration& config);
	void loadFiles();
	bool canOpenAdminFile();
	Tree* createAdminFile();
	Tree* getAdminFile();
	int getVoterBlockAmount();
	int getChargeBlockAmount();
	void calculateBlockAmounts();
	~DataFileLoader();
};

#endif /* DATAFILELOADER_H_ */
