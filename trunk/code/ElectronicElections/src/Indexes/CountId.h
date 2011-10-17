/*
 * CountId.h
 *
 *  Created on: Oct 17, 2011
 *      Author: damian
 */

#ifndef COUNTID_H_
#define COUNTID_H_

#include <string>
class CountId {
	char day;
	char month;
	short year;
	std::string charge;
	std::string listName;
public:
	CountId();
	CountId(char day, char month, short year, std::string& charge, std::string& listName);
	char getDay();
	void setDay(char value);
	char getMonth();
	void setMonth(char value);
	short getYear();
	void setYear(short value);
	std::string& getCharge();
	void setCharge(std::string& value);
	std::string& getListName();
	void setListName(std::string& value);
	virtual ~CountId();
};

#endif /* COUNTID_H_ */
