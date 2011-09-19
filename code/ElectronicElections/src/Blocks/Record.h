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
	bool isEmpty;
	bool wasDeleted;
public:
	Record(int size);
	Record(Record& other);
	Record& operator=(const Record& other);
	char* getBytes();
	void setBytes(const char* value);
	void setIsEmpty(bool value);
	void setWasDeleted(bool value);
	bool getIsEmpty();
	bool getWasDeleted();
	int getSize();
	virtual ~Record();
};

#endif /* RECORD_H_ */

