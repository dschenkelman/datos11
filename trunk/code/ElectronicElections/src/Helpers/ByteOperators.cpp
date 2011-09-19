/*
 * ByteOperators.cpp
 *
 *  Created on: Sep 18, 2011
 *      Author: damian
 */

#include "ByteOperators.h"


void ByteOperators::setBit(char* byte, char position, char value)
{
	// bit 0 is the left most
	if (position <0 || position > 7)
		return;
	if (value == 1)
	{
		*byte |= 1 << (7- position);
	}
	else if (value == 0)
	{
		*byte &= ~(1 << (7 - position));
	}
}

bool ByteOperators::isBitOne(char byte, char position)
{
	// bit 0 is the left most
	if (position < 0 || position > 7)
		return false;
	return byte & (1 << (7 - position));
}


