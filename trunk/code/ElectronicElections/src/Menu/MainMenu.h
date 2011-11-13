/*
 * MainMenu.h
 *
 *  Created on: 09/11/2011
 *      Author: lejosdelcielo
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_

#include <iostream>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <sstream>
#include "../Indexes/CountId.h"
#include "../Voting/Configuration.h"
#include "../Voting/DataFileLoader.h"

enum UserAction
{
	InvalidUserOption = -1,
	Login = 0,
	Quit = 1
};

enum AdminAction
{
	InvalidAdminOption = -1,
	PopulateFiles = 0,
	GenerateVotes = 1,
	DistrictABM = 2,
	VoterABM = 3,
	ElectionABM = 4,
	ChargeABM = 5,
	ElectionListABM = 6,
	CandidateABM = 7,
	AdministratorABM = 8,
	ReportResults = 9,
	UpdateCount = 10,
	ReportBack = 11,
	DecryptReport = 12,
	ReturnBack = 13
};

class MainMenu
{
private:
	Configuration configuration;
	DataFileLoader dataFileLoader;
	Log log;
	string askPassword();
	void saveReport(stringstream& report);
	void enterValidDate(short* year, char* month, char* day);
	static bool countIdCmp(CountId c1, CountId c2);
	bool login();
	void populateFiles();
	void generateVotes();
	void districtABM();
	void voterABM();
	void electionABM();
	void chargeABM();
	void electionListABM();
	void candidateABM();
	void administratorABM();
	void reportResults();
	void updateCount();
	void recoverFile();
	void decryptReport();
public:
	MainMenu(string& configurationFilePath);
	void runTests();
	void runApplication();
	virtual ~MainMenu();
};

#endif /* MAINMENU_H_ */
