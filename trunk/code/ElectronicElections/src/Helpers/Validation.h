/*
 * Validation.h
 *
 *  Created on: 06/11/2011
 *      Author: lejosdelcielo
 */

#ifndef VALIDATION_H_
#define VALIDATION_H_

class Validation {
private:
	bool isLeapYear(short year);
	bool isValidMonth(char month);
	bool isValidDay(char day, char month);
	bool isInArray(char month, char months[], int arraySize);
public:
	Validation();
	bool isValidDate(short year, char month, char day);
	virtual ~Validation();
};

#endif /* VALIDATION_H_ */
