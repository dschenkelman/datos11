/*
 * StringHelpers.h
 *
 *  Created on: Nov 13, 2011
 *      Author: damian
 */

#ifndef STRINGHELPERS_H_
#define STRINGHELPERS_H_

#include <string>

class StringHelpers {
public:
	StringHelpers();
	static std::string toUpper(std::string s);
	virtual ~StringHelpers();
};

#endif /* STRINGHELPERS_H_ */
