/*
 * RecordComparer.h
 *
 *  Created on: Sep 11, 2011
 *      Author: damian
 */

#ifndef RECORDCOMPARER_H_
#define RECORDCOMPARER_H_

// used to compare records by primary key
class RecordComparer
{
public:
	// returns 0 if the keys are equal
	// > 0 => if key is greater than key in recordBytes
	// < 0 => if key in recordBytes is greater than key
	virtual int compare(const char* key, const char* recordBytes, int recordSize) = 0;
};

#endif /* RECORDCOMPARER_H_ */
