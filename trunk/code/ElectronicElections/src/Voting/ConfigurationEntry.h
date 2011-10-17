/*
 * ConfigurationEntry.h
 *
 *  Created on: Oct 16, 2011
 *      Author: damian
 */

#ifndef CONFIGURATIONENTRY_H_
#define CONFIGURATIONENTRY_H_

#include <string>

class ConfigurationEntry
{
public:
	ConfigurationEntry();
	ConfigurationEntry(std::string fName, std::string dName, int bSize, int rCount, int rSize);
	std::string loadFileName;
	std::string dataFileName;
	int blockSize;
	int estimatedRegisterCount;
	int estimateRegisterSize;
	int getBlockSize();
	int getRegisterSize();
	int getRegisterCount();
	std::string& getLoadFileName();
	std::string& getDataFileName();
	virtual ~ConfigurationEntry();
};

#endif /* CONFIGURATIONENTRY_H_ */
