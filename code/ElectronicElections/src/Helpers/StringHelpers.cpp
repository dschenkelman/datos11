/*
 * StringHelpers.cpp
 *
 *  Created on: Nov 13, 2011
 *      Author: damian
 */

#include "StringHelpers.h"
#include <cctype>

StringHelpers::StringHelpers() {
}

std::string StringHelpers::toUpper(std::string s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if (isalpha(s[i]))
		{
			s[i] = toupper(s[i]);
		}
	}

	return s;
}

StringHelpers::~StringHelpers() {
}
