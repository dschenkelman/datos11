/*
 * IndexTreeBlock.cpp
 *
 *  Created on: Sep 27, 2011
 *      Author: juanma
 */

#include "IndexTreeBlock.h"

IndexTreeBlock::IndexTreeBlock(int size, RecordMethods* methods): TreeBlock(size,0,0,methods)
{
	// TODO Auto-generated constructor stub

}

IndexTreeBlock::~IndexTreeBlock() {
	// TODO Auto-generated destructor stub
}

bool IndexTreeBlock::insertRecord(VariableRecord* keyRecord, VariableRecord* dataRecord)
{
	return false;
}
