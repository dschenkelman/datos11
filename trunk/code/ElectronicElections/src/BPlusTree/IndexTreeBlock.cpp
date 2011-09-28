/*
 * IndexTreeBlock.cpp
 *
 *  Created on: Sep 27, 2011
 *      Author: juanma
 */

#include "IndexTreeBlock.h"
#include <string.h>

IndexTreeBlock::IndexTreeBlock(int size, RecordMethods* methods)
: TreeBlock(size,RECORD_OFFSET,RECORD_OFFSET,methods)
{
	firstPointerPosition = -1;
}

IndexTreeBlock::~IndexTreeBlock()
{
}

// WIP!!!!
bool IndexTreeBlock::insertRecord(VariableRecord* keyRecord, VariableRecord* dataRecord)
{
	bool theKeyisLowerThanTheSeparator = false;
	short childNumber = 0;
	this->position = RECORD_OFFSET;
	while(this->position < this->firstPointerPosition && !theKeyisLowerThanTheSeparator)
	{
		short separatorSize;
		memcpy(&separatorSize, this->bytes + this->position, SEPARATOR_HEADER_SIZE);

		//this->recordMethods->compare()
		if (1)	//comparation	COMPARE KEYS!	(true if key is lower than separator)
		{
			theKeyisLowerThanTheSeparator = true;
		}
		else
		{
			childNumber++;
			this->position += SEPARATOR_HEADER_SIZE + separatorSize;
		}

	}
	if (!theKeyisLowerThanTheSeparator)
	{
		// (Insert new Separator) || (Overflow)
		return false;
	}

	return false;
}

bool IndexTreeBlock::insertSeparator(const char* key, short len)
{
	return false;
}
