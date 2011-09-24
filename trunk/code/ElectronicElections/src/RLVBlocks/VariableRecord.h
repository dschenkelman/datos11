/*
 * VariableRecord.h
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#ifndef VARIABLERECORD_H_
#define VARIABLERECORD_H_

class VariableRecord
{
	int size;
	char* bytes;
public:
	VariableRecord();
	VariableRecord(VariableRecord& other);
	VariableRecord& operator=(const VariableRecord& other);
	char* getBytes();
	void setBytes(const char* value, int len);
	int getSize();
	virtual ~VariableRecord();
};

#endif /* VARIABLERECORD_H_ */
