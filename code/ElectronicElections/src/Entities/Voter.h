/*
 * Voter.h
 *
 *  Created on: 07/10/2011
 *      Author: lejosdelcielo
 */

#ifndef VOTER_H_
#define VOTER_H_

#include <string>
#include <vector>

using namespace std;

struct ElectionKey
{
	short year;
	char month;
	char day;
	std::string charge;
};

class Voter {
private:
	int dni;
	std::string names;
	std::string password;
	std::string address;
	std::string district;
	std::vector<ElectionKey> electionKeyList;
	char* bytes;
	char* key;
public:
	Voter(int dni, std::string names, std::string password, std::string address,
			std::string district, std::vector<ElectionKey> electionKeyList);
	char* getBytes();
	int getSize();
	char* getKey();
	int getKeySize();
	void setBytes(char* bytes);
	virtual ~Voter();
	//getters
	int getDni();
	std::string getNames();
	std::string getPassword();
	void setPassword(std::string password);
	std::string getAddress();
	void setAddress(std::string address);
	std::string getDistrict();
	void setDistrict(std::string district);
	std::vector<ElectionKey> getElectionKeyList();
};

#endif /* VOTER_H_ */
