/*
 * Administrator.h
 *
 *  Created on: 04/10/2011
 *      Author: lejosdelcielo
 */

#ifndef ADMINISTRATOR_H_
#define ADMINISTRATOR_H_

static const int PASSWORD_SIZE = 4;

#include <string>
#include <vector>

class Administrator {
private:
	std::string name;
	std::vector<char> password;
	char* bytes;
	char* key;
public:
	Administrator(std::string name, std::vector<char> password);
	Administrator(const Administrator& other);
	Administrator& operator=(const Administrator& other);
	char* getBytes();
	int getSize();
	char* getKey();
	int getKeySize();
	void setBytes(char* bytes);
	virtual ~Administrator();
	// getters
	std::string getName();
	std::vector<char> getPassword();
	void setPassword(std::vector<char>& value);
};

#endif /* ADMINISTRATOR_H_ */
