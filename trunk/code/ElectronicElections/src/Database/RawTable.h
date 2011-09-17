/*
 * RawTable.h
 *
 *  Created on: 17/09/2011
 *      Author: alejandro
 */

#ifndef RAWTABLE_H_
#define RAWTABLE_H_

#include <string>
#include "RawTableRow.h"
using namespace std;

class RawTable {
public:
	RawTable(string name);
	bool insert(char* key, int key_size, char* value, int value_size);
	RawTableRow select(char* key, int key_size);
	virtual ~RawTable();
};

#endif /* RAWTABLE_H_ */
