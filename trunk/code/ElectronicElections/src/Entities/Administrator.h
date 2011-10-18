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

class Administrator {
private:
	std::string name;
	std::string password;
	char* bytes;
	char* key;
public:
	Administrator(std::string name, std::string password);
	Administrator(Administrator& other);
	Administrator& operator=(Administrator& other);
	char* getBytes();
	int getSize();
	char* getKey();
	int getKeySize();
	void setBytes(char* bytes);
	virtual ~Administrator();
	// getters
	std::string getName();
	std::string getPassword();
	void setPassword(std::string& value);
};

#endif /* ADMINISTRATOR_H_ */
