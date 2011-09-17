/*
 * RawTableRow.h
 *
 *  Created on: 17/09/2011
 *      Author: alejandro
 *
 */

#ifndef RAWTABLEROW_H_
#define RAWTABLEROW_H_

#include <iostream>

class RawTableRow {
public:
	char *key, *value;
	int key_size, value_size;
	RawTableRow(char* key, int key_size, char* value, int value_size);
	virtual ~RawTableRow();
};

#endif /* RAWTABLEROW_H_ */
