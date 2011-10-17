/*
 * ConfigurationEntry.cpp
 *
 *  Created on: Oct 16, 2011
 *      Author: damian
 */

#include "ConfigurationEntry.h"

ConfigurationEntry::ConfigurationEntry()
:loadFileName(""), blockSize(0), estimatedRegisterCount(0),estimateRegisterSize(0), dataFileName("")
{
}

ConfigurationEntry::ConfigurationEntry(std::string fName, std::string dName, int bSize, int rCount, int rSize)
:loadFileName(fName), dataFileName(dName), blockSize(bSize), estimatedRegisterCount(rCount),estimateRegisterSize(rSize)
{
}

int ConfigurationEntry::getBlockSize()
{
	return this->blockSize;
}

int ConfigurationEntry::getRegisterCount()
{
	return this->estimatedRegisterCount;
}

int ConfigurationEntry::getRegisterSize()
{
	return this->estimateRegisterSize;
}

std::string& ConfigurationEntry::getLoadFileName()
{
	return this->loadFileName;
}

std::string& ConfigurationEntry::getDataFileName()
{
	return this->dataFileName;
}

ConfigurationEntry::~ConfigurationEntry()
{

}
