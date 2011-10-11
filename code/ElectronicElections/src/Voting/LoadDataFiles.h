/*
 * LoadDataFiles.h
 *
 *  Created on: 09/10/2011
 *      Author: gabriel
 */

#ifndef LOADDATAFILES_H_
#define LOADDATAFILES_H_
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


class LoadDataFiles
{
	std::string configName;
	std::ifstream configFile;
	// District attributes
	std::string districtFileName;
	int districtBlockSize;
	// Election attributes
	std::string electionFileName;
	int electionBlockSize;
	// ElectionList attributes
	std::string electionListFileName;
	int electionListBlockSize;
	// Count attributes
	std::string countFileName;
	int countBlockSize;
	// Candidate attributes
	std::string candidateFileName;
	int candidateBlockSize;
	// Administrator attributes
	std::string administratorFileName;
	int administratorBlockSize;
	// Voter attributes
	std::string voterFileName;
	int voterBlockSize;
	int voterBlockAmount;
	// Charge attributes
	std::string chargeFileName;
	int chargeBlockSize;
	int chargeBlockAmount;

	//void initializefile();

	void readDistrictFile(Tree* treeDistrictFile, char* dataFile);
	void readVoterFile(HashBlockFile* hashVoterFile, char* dataFile);
	void readChargeFile(HashBlockFile* hashChargeFile, char* dataFile);
	void createFileType(char* fileType,char** fields);

public:
	LoadDataFiles(std::string configFileName);
	bool canOpenAdminFile();
	Tree createAdminFile();
	Tree getAdminFile();

	string getChargeFileName();
	string getVoterFileName();
	string getDistrictFileName();
	string getElectionFileName();
	string getElectionListFileName();
	string getCountFileName();
	string getCandidateFileName();
	string getAdminFileName();

	int getDistrictBlockSize();
	int getElectionBlockSize();
	int getElectionListBlockSize();
	int getCountBlockSize();
	int getCandidateBlockSize();
	int getAdminBlockSize();

	int getVoterBlockSize();
	int getVoterBlockAmount();
	int getChargeBlockSize();
	int getChargeBlockAmount();

	void readConfigFile();
	~LoadDataFiles();
};

#endif /* LOADDATAFILES_H_ */
