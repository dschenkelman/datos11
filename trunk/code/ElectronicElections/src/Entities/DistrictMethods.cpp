/*
 * DistrictMethods.cpp
 *
 *  Created on: 28/09/2011
 *      Author: alejandro
 */

#include "DistrictMethods.h"

//DEFINITION:
//char[FIELD_HEADER_SIZE] name_size
//char[name_size] name // ending with \0


DistrictMethods::DistrictMethods() {
	// TODO Auto-generated constructor stub

}

DistrictMethods::~DistrictMethods() {
	// TODO Auto-generated destructor stub
}

District* DistrictMethods::getObject(VariableRecord record) {
	char* bytes = record.getBytes();
	District* ret = new District(bytes+Constants::FIELD_HEADER_SIZE);
	return ret;
}
