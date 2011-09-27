/*
 * VoterIndexRecordMethods.h
 *
 *  Created on: Sep 23, 2011
 *      Author: damian
 */

#ifndef VOTERINDEXMETHODS_H_
#define VOTERINDEXMETHODS_H_

#include "../VariableBlocks/RecordMethods.h"
#include "VoterIndex.h"

class VoterIndexMethods: public RecordMethods
{
	VoterIndex getVoterIndexFromRecord(const char *recordBytes, int recordSize);
public:
	VoterIndexMethods();
	virtual int compare(const char* key, const char* recordBytes, int recordSize);
	virtual void print(const char* recordBytes, int recordSize);
	virtual ~VoterIndexMethods();
};

#endif /* VOTERINDEXRECORDMETHODS_H_ */
