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

class DistrictElectionsIndex
{
	std::string fileName;
	Tree* file;
public:
	DistrictElectionsIndex(int blockSize, bool createNew);
	void addElectionToDistrict(std::string district, char day, char month, char year, std::string charge);
	bool getDistrictElections(std::string district, VariableRecord* record);

	virtual ~DistrictElectionsIndex();
};

#endif /* DISTRICTELECTIONSINDEX_H_ */
