/*
 * Configuration.h
 *
 *  Created on: Oct 16, 2011
 *      Author: damian
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string>
#include <map>
#include "ConfigurationEntry.h"

class Configuration
{
	std::string fileName;
	std::map<std::string, ConfigurationEntry> entries;
	int keySize;
public:
	Configuration(std::string fName);
	void read();
	ConfigurationEntry& getEntry(std::string name);
	virtual ~Configuration();
};

#endif /* CONFIGURATION_H_ */
