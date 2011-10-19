/*
 * DistrictCountsIndex.h
 *
 *  Created on: Oct 17, 2011
 *      Author: damian
 */

#ifndef DISTRICTCOUNTSINDEX_H_
#define DISTRICTCOUNTSINDEX_H_

#include <string>
#include "../BPlusTree/Tree.h"
#include "../Entities/Count.h"
#include "DistrictCountsMethods.h"

class DistrictCountsIndex {
	DistrictCountsMethods* methods;
	std::string fileName;
	Tree* file;
	void addCountToDistrict(std::string district, char day, char month, short year, std::string charge, std::string listName);
	void removeCountFromDistrict(std::string district, char day, char month, short year, std::string charge, std::string listName);
public:
	DistrictCountsIndex(std::string& fName, int blockSize, bool createNew);
	void indexCount(Count& c);
	void unIndexCount(Count& c);
	bool getDistrictCounts(std::string district, VariableRecord* record);
	virtual ~DistrictCountsIndex();
};

#endif /* DISTRICTCOUNTSINDEX_H_ */
