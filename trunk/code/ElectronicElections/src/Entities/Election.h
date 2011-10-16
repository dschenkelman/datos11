/*
 * Election.h
 *
 *  Created on: 06/10/2011
 *      Author: lejosdelcielo
 */

#ifndef ELECTION_H_
#define ELECTION_H_

#include <string>
#include <vector>

using namespace std;

class Election {
private:
	char day;
	char month;
	short year;
	std::string charge;
	std::vector<std::string> districtList;
	char* bytes;
	char* key;
public:
	Election(char day, char month, short year, std::string charge);
	Election(char day, char month, short year, std::string charge, vector<string>& districts);
	char* getBytes();
	int getSize();
	char* getKey();
	int getKeySize();
	void setBytes(char* bytes);
	virtual ~Election();
	//getters
	char getDay();
	char getMonth();
	short getYear();
	std::string& getCharge();
	std::vector<std::string>& getDistrictList();
};

#endif /* ELECTION_H_ */
