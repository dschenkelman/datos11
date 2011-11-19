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
		char* fileType = strtok((char*)line.c_str(), ",");
		for(int i = 0; i<5; i++)
		{
			fields[i] = strtok(NULL, ",\r");
		}

		if (strcmp(fileType, "KeySize") == 0) { // si no es entidad
			this->keySize = atoi(fields[0]);
			if (this->keySize > 8) {
				cout << "WARNING: Key Size exceeds 8 bytes" << endl;
			} else if (this->keySize < 2) {
				cout << "WARNING: Key Size too small" << endl;
			}
		} else { // si es entidad
			ConfigurationEntry entry(fields[0], fields[4], atoi(fields[1]), atoi(fields[2]), atoi(fields[3]));
			string entryName = fileType;
			this->entries[entryName] = entry;
		}
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
