/*
 * Candidate.h
 *
 *  Created on: 04/10/2011
 *      Author: lejosdelcielo
 */

#ifndef CANDIDATE_H_
#define CANDIDATE_H_

#include <string>


class Candidate {
private:
	char* bytes;
	short year;
	char month;
	char day;
	std::string charge;
	std::string listName;
	int dni;
public:
	Candidate(char day, char month, short year, std::string name, std::string charge, int dni);
	Candidate(const Candidate& other);
	Candidate& operator=(const Candidate& other);
	char* getBytes();
	int getSize();
	char* getKey();
	int getKeySize();
	void setBytes(char* bytes);
	virtual ~Candidate();
	//getters
	short getYear();
	char getMonth();
	char getDay();
	std::string getCharge();
	std::string getListName();
	int getDni();
};

#endif /* CANDIDATE_H_ */
