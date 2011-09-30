/*
 * District.h
 *
 *  Created on: 27/09/2011
 *      Author: gonzalo
 */

#ifndef DISTRICT_H_
#define DISTRICT_H_

#include <string>

using namespace std;

class District {
private:
	std::string name;
	char* bytes;
public:
	District(std::string name);
	char* getBytes();
	int getSize();
	char* getKey();
	int getKeySize();
	void setBytes(char* bytes);
	std::string getName();
	virtual ~District();
};

#endif /* DISTRICT_H_ */
