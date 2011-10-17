/*
 * DistrictElectionsIndex.cpp
 *
 *  Created on: Oct 16, 2011
 *      Author: damian
 */

#include "DistrictElectionsIndex.h"
#include "DistrictElections.h"

using namespace std;

DistrictElectionsIndex::DistrictElectionsIndex(string& fName, int blockSize, bool createNew)
:fileName(fName)
{
	this->methods = new DistrictElectionsMethods;
	this->file = new Tree(this->fileName,blockSize, this->methods, createNew);
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

void DistrictElectionsIndex::removeElectionFromDistrict(std::string district, char day, char month, char year, std::string charge)
{
	DistrictElections de(district);
	VariableRecord record;
	if (this->file->get(de.getKey(), &record))
	{
		de.setBytes(record.getBytes());
		de.removeElection(day, month, year, charge);
		if (de.hasElections())
		{
			record.setBytes(de.getBytes(), de.getSize());
			this->file->update(de.getKey(), &record);
		}
		else
		{
			this->file->remove(de.getKey());
		}
	}
}

void DistrictElectionsIndex::unIndexElection(Election & e)
{
	for(unsigned int i = 0; i < e.getDistrictList().size(); i++)
	{
		string d = e.getDistrictList().at(i);
		this->removeElectionFromDistrict(d, e.getDay(),
				e.getMonth(), e.getYear(), e.getCharge());
	}
}

bool DistrictElectionsIndex::getDistrictElections(std::string district, VariableRecord *record)
{
	DistrictElections de(district);
	return this->file->get(de.getKey(), record);
}

void DistrictElectionsIndex::indexElection(Election & e)
{
	for(unsigned int i = 0; i < e.getDistrictList().size(); i++)
	{
		string d = e.getDistrictList().at(i);
		this->addElectionToDistrict(d, e.getDay(),
				e.getMonth(), e.getYear(), e.getCharge());
	}
}

DistrictElectionsIndex::~DistrictElectionsIndex()
{
	delete this->file;
	delete this->methods;
}
