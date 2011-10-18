/*
 * DistrictCounts.h
 *
 *  Created on: Oct 17, 2011
 *      Author: damian
 */

#ifndef DISTRICTCOUNTS_H_
#define DISTRICTCOUNTS_H_

#include <vector>
#include <string>
#include "CountId.h"

class DistrictCounts
{
	std::vector<CountId> counts;
	std::string district;
	char* bytes;
	char* key;
public:
	DistrictCounts();
	DistrictCounts(std::string& d);
	DistrictCounts(DistrictCounts& other);
	DistrictCounts& operator=(DistrictCounts& other);
	int getSize();
	int getKeySize();
	void setBytes(const char* value);
	char* getKey();
	char* getBytes();
	std::string& getDistrict();
	void setDistrict(std::string& value);
	void addCount(char d, char m, short y, std::string& c, std::string& lName);
	void removeCount(char d, char m, short y, std::string& c, std::string& lName);
	bool hasCounts();
	virtual ~DistrictCounts();
};

#endif /* DISTRICTCOUNTS_H_ */
