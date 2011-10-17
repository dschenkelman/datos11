/*
 * Configuration.cpp
 *
 *  Created on: Oct 16, 2011
 *      Author: damian
 */

#include "Configuration.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

Configuration::Configuration(std::string fName) : fileName(fName)
{

}

void Configuration::read()
{
	ifstream configFile;
	configFile.open(this->fileName.c_str(), ios::in);
	if (!configFile.is_open())
	{
		cerr << this->fileName + " file could not be found!" << endl;
		exit(1);
	}
	std::string line;
	char* fields[5];
	while (getline(configFile,line))
	{
		char l[line.length() + 1];
		strcpy(l, line.c_str());
		char* fileType = strtok(l, ",");
		for(int i = 0; i<5; i++)
		{
			fields[i] = strtok(NULL, ",\r");
		}

		ConfigurationEntry entry(fields[0], fields[4], atoi(fields[1]), atoi(fields[2]), atoi(fields[3]));
		string entryName = fileType;
		this->entries[entryName] = entry;
	}
	configFile.close();
}

ConfigurationEntry & Configuration::getEntry(std::string name)
{
	return this->entries[name];
}

Configuration::~Configuration()
{
}
