/*
 * ByteOperators.cpp
 *
 *  Created on: Sep 18, 2011
 *      Author: damian
 */

#include "ByteOperators.h"


void ByteOperators::setBit(char* byte, char bit, char value)
{
	if (bit <0 || bit > 7)
		return;
	if (value == 1)
	{
		*byte |= 1 << bit;
	}
	else if (value == 0)
	{
		*byte &= ~(1 << bit);
	}
}

bool ByteOperators::isBitOne(char byte, int position)
{
	if (position < 0 || position > 7)
		return false;
	return byte & (1 << position);
}


