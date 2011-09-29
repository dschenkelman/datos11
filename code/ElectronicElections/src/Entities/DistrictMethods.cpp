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

}

DistrictMethods::~DistrictMethods() {
}

District* DistrictMethods::getObject(VariableRecord* record) {
	char* bytes = record->getBytes();
	District* ret = new District(bytes+Constants::FIELD_HEADER_SIZE);
	return ret;
}

VariableRecord* DistrictMethods::getVariableRecord(District* district) {
	// en este caso la serializacion esta definida en la entidad
	return new VariableRecord(district->getBytes(), district->getSize());
}
