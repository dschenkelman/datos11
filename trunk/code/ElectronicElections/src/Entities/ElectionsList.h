/*
 * ElectionsList.h
 *
 *  Created on: 30/09/2011
 *      Author: lejosdelcielo
 */

#ifndef ELECTIONSLIST_H_
#define ELECTIONSLIST_H_
#include "../VariableBlocks/Constants.h"
#include <cstring>
#include <string>

class ElectionsList {
private:
	char* bytes;
	std::string name;
	char day;
	char month;
	short year;
	std::string charge;
public:
	ElectionsList(std::string name, char day, char month, short year, std::string charge);
	int getSize();
	char* getKey();
	int getKeySize();
	char* getBytes();
	void setBytes(char* bytes);
	virtual ~ElectionsList();
};

#endif /* ELECTIONSLIST_H_ */
