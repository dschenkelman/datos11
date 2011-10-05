/*
 * Count.h
 *
 *  Created on: 05/10/2011
 *      Author: lejosdelcielo
 */

#ifndef COUNT_H_
#define COUNT_H_

#include <string>

class Count {
private:
	char* bytes;
	short year;
	char month;
	char day;
	std::string charge;
	std::string name;
	std::string district;
	unsigned int quantity;
public:
	Count(char day, char month, short year, std::string charge, std::string name, std::string district, unsigned int quantity);
	char* getBytes();
	int getSize();
	char* getKey();
	int getKeySize();
	void setBytes(char* bytes);
	virtual ~Count();
};

#endif /* COUNT_H_ */
