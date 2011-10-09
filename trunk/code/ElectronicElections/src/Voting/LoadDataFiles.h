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
#include "../Hash/VoterHashingFunction.h"
#include "../Hash/ChargeHashingFunction.h"
#include "../Entities/ChargeMethods.h"
#include "../Entities/VoterMethods.h"
#include "../Entities/DistrictMethods.h"

class LoadDataFiles
{
	std::string configName;
	std::ifstream configFile;
	HashBlockFile* hashVoterFile;
	HashBlockFile* hashChargeFile;
	Tree* treeDistrictFile;

	//void initializefile();
	void readDistrictFile();
	void readVoterFile();
	void readChargeFile();
	void createFileType(char* fileType,char** fields);

public:
	LoadDataFiles(std::string configFileName);

	void readConfigFile();
	~LoadDataFiles();
};

#endif /* LOADDATAFILES_H_ */
