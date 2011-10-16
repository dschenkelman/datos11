/*
 * DistrictElections.h
 *
 *  Created on: Oct 16, 2011
 *      Author: damian
 */

#ifndef DISTRICTELECTIONS_H_
#define DISTRICTELECTIONS_H_

#include <vector>
#include <string>
#include "ElectionId.h"

class DistrictElections
{
	std::vector<ElectionId> elections;
	std::string district;
	char* bytes;
	char* key;
public:
	DistrictElections();
	DistrictElections(std::string& d);
	int getSize();
	int getKeySize();
	void setBytes(const char* value);
	char* getKey();
	char* getBytes();
	std::string& getDistrict();
	void setDistrict(std::string& value);
	void addElection(char d, char m, short y, std::string& c);
	virtual ~DistrictElections();
};

#endif /* DISTRICTELECTIONS_H_ */
