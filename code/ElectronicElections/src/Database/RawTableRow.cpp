/*
 * RawTableRow.cpp
 *
 *  Created on: 17/09/2011
 *      Author: alejandro
 */

#include "RawTableRow.h"

RawTableRow::RawTableRow(char* key, int key_size, char* value, int value_size) {
	this->key = key;
	this->key_size = key_size;
	this->value = value;
	this->value_size = value_size;
}

RawTableRow::~RawTableRow() {
	delete this->key;
	delete this->value;
}
