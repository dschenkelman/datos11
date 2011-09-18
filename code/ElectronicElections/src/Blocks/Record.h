/*
 * Record.h
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#ifndef RECORD_H_
#define RECORD_H_

class Record
{
	int size;
	char* bytes;
	bool free;
	bool erased;

public:
	Record();
	Record(Record& other);
	Record& operator=(const Record& other);
	char* getBytes();
	void setBytes(const char* value, int len);
	int getSize();
	bool isFree();
	bool isErased();
	void setRecordStatus(bool freeRecord, bool erasedRecord);
	virtual ~Record();
};

#endif /* RECORD_H_ */
