/*
 * Constants.h
 *
 *  Created on: Sep 14, 2011
 *      Author: damian
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

class Constants
{
public:
	static const int BLOCK_HEADER_SIZE = sizeof(int);
	static const int RECORD_HEADER_SIZE = sizeof(short);
	static const int FIELD_HEADER_SIZE = sizeof(char);
};

#endif /* CONSTANTS_H_ */
