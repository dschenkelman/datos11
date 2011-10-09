/*
 * CandidatesList.h
 *
 *  Created on: 08/10/2011
 *      Author: alejandro
 */

#ifndef CANDIDATESLIST_H_
#define CANDIDATESLIST_H_

#include <string>

class CandidatesList {
private:
	char* bytes;
	char* key;
	short year;
	char month;
	char day;
	std::string charge;
	std::string name;
public:
	CandidatesList(char day, char month, short year, std::string charge, std::string name);
	char* getBytes();
	int getSize();
	char* getKey();
	int getKeySize();
	void setBytes(char* bytes);
	virtual ~CandidatesList();
	//getters
	short getYear();
	char getMonth();
	char getDay();
	std::string getCharge();
	std::string getListName();
};

#endif /* CANDIDATESLIST_H_ */
