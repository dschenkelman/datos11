/*
 * ElectionId.h
 *
 *  Created on: Oct 16, 2011
 *      Author: damian
 */

#ifndef ELECTIONID_H_
#define ELECTIONID_H_

#include <string>

class ElectionId
{
	char day;
	char month;
	short year;
	std::string charge;
public:
	ElectionId();
	ElectionId(char day, char month, short year, std::string& charge);
	char getDay();
	void setDay(char value);
	char getMonth();
	void setMonth(char value);
	short getYear();
	void setYear(short value);
	std::string& getCharge();
	void setCharge(std::string& value);
	virtual ~ElectionId();
};

#endif /* ELECTIONID_H_ */
