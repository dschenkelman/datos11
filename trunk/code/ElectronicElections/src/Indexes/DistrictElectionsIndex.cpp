/*
 * DistrictElectionsIndex.cpp
 *
 *  Created on: Oct 16, 2011
 *      Author: damian
 */

#include "DistrictElectionsIndex.h"
#include "DistrictElectionMethods.h"
#include "DistrictElections.h"

DistrictElectionsIndex::DistrictElectionsIndex(int blockSize, bool createNew)
{
	this->fileName = "districtElections.ind";
	DistrictElectionsMethods methods;
	this->file = new Tree(this->fileName,blockSize, &methods, createNew);
}

void DistrictElectionsIndex::addElectionToDistrict(std::string district, char day, char month, char year, std::string charge)
{
	DistrictElections de(district);
	VariableRecord record;
	if (this->file->get(de.getKey(), &record))
	{
		de.setBytes(record.getBytes());
		de.addElection(day, month, year, charge);
		record.setBytes(de.getBytes(), de.getSize());
		this->file->update(de.getKey(), &record);
	}
	else
	{
		de.addElection(day, month, year, charge);
		record.setBytes(de.getBytes(), de.getSize());
		VariableRecord keyRecord;
		keyRecord.setBytes(de.getKey(), de.getKeySize());
		this->file->insert(&keyRecord, &record);
	}
}

bool DistrictElectionsIndex::getDistrictElections(std::string district, VariableRecord *record)
{
	DistrictElections de(district);
	return this->file->get(de.getKey(), record);
}

DistrictElectionsIndex::~DistrictElectionsIndex()
{
	delete this->file;
}
