/*
 * DistrictElectionsIndex.h
 *
 *  Created on: Oct 16, 2011
 *      Author: damian
 */

#ifndef DISTRICTELECTIONSINDEX_H_
#define DISTRICTELECTIONSINDEX_H_

#include "../BPlusTree/Tree.h"
#include <string>
#include "../Entities/Election.h"
#include "DistrictElectionsMethods.h"

class DistrictElectionsIndex
{
	std::string fileName;
	Tree* file;
	DistrictElectionsMethods* methods;
	void addElectionToDistrict(std::string district, char day, char month, char year, std::string charge);
	void removeElectionFromDistrict(std::string district, char day, char month, char year, std::string charge);
public:
	DistrictElectionsIndex(std::string& fName, int blockSize, bool createNew);
	void indexElection(Election& e);
	void unIndexElection(Election& e);
	bool getDistrictElections(std::string district, VariableRecord* record);

	virtual ~DistrictElectionsIndex();
};

#endif /* DISTRICTELECTIONSINDEX_H_ */
