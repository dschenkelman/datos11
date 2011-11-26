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
#include "../RSA/RSACipher.h"
#include "../RSA/RSAKey.h"

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
	std::vector<char> password;
	std::string address;
	std::string district;
	std::vector<ElectionKey> electionKeyList;
	char* bytes;
	char* key;
public:
	Voter(int dni, std::string names, std::vector<char> password, std::string address,
			std::string district, std::vector<ElectionKey> electionKeyList);
	Voter(int dni, std::string names, std::vector<char> password, std::string address,
				std::string district);
	Voter(const Voter& other);
	Voter& operator=(const Voter& other);
	char* getBytes();
	int getSize();
	char* getKey();
	int getKeySize();
	void setBytes(char* bytes);
	/* ENCRYPTION */
	void setEncBytes(char* bytes, RSAKey privateKey, int originalSize);
	void getEncBytes(char* encBytes, RSAKey publicKey);
	/* END ENCRYPTION */
	virtual ~Voter();
	//getters
	int getDni();
	std::string getNames();
	void setNames(std::string names);
	std::vector<char> getPassword();
	void setPassword(std::vector<char> password);
	std::string getAddress();
	void setAddress(std::string address);
	std::string getDistrict();
	void setDistrict(std::string district);
	std::vector<ElectionKey>& getElectionKeyList();
};

#endif /* VOTER_H_ */
