/*
 * DistrictCountsIndex.cpp
 *
 *  Created on: Oct 17, 2011
 *      Author: damian
 */

#include "DistrictCountsIndex.h"
#include "DistrictCounts.h"

using namespace std;

DistrictCountsIndex::DistrictCountsIndex(string& fName, int blockSize, bool createNew)
:fileName(fName)
{
	this->methods = new DistrictCountsMethods;
	this->file = new Tree(this->fileName,blockSize, this->methods, createNew);
}

bool DistrictCountsIndex::getDistrictCounts(string district, VariableRecord *record)
{
	DistrictCounts dc(district);
	return this->file->get(dc.getKey(), record);
}

void DistrictCountsIndex::indexCount(Count & c)
{
	this->addCountToDistrict(c.getDistrict(), c.getDay(),
				c.getMonth(), c.getYear(), c.getCharge(), c.getListName());
}

void DistrictCountsIndex::addCountToDistrict(string district, char day, char month, short year, string charge, string listName)
{
	DistrictCounts dc(district);
	VariableRecord record;
	if (this->file->get(dc.getKey(), &record))
	{
		dc.setBytes(record.getBytes());
		dc.addCount(day, month, year, charge, listName);
		record.setBytes(dc.getBytes(), dc.getSize());
		this->file->update(dc.getKey(), &record);
	}
	else
	{
		dc.addCount(day, month, year, charge, listName);
		record.setBytes(dc.getBytes(), dc.getSize());
		VariableRecord keyRecord;
		keyRecord.setBytes(dc.getKey(), dc.getKeySize());
		this->file->insert(&keyRecord, &record);
	}
}

void DistrictCountsIndex::removeCountFromDistrict(string district, char day, char month, short year, string charge, string listName)
{
	DistrictCounts dc(district);
	VariableRecord record;
	if (this->file->get(dc.getKey(), &record))
	{
		dc.setBytes(record.getBytes());
		dc.removeCount(day, month, year, charge, listName);
		if (dc.hasCounts())
		{
			record.setBytes(dc.getBytes(), dc.getSize());
			this->file->update(dc.getKey(), &record);
		}
		else
		{
			this->file->remove(dc.getKey());
		}
	}
}

void DistrictCountsIndex::unIndexCount(Count & c)
{
	this->removeCountFromDistrict(c.getDistrict(), c.getDay(),
			c.getMonth(), c.getYear(), c.getCharge(), c.getListName());
}

DistrictCountsIndex::~DistrictCountsIndex() {
	delete this->file;
	delete this->methods;
}
