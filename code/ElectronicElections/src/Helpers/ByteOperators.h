/*
 * ByteOperators.h
 *
 *  Created on: Sep 18, 2011
 *      Author: damian
 */

#ifndef BYTEOPERATORS_H_
#define BYTEOPERATORS_H_

class ByteOperators
{
public:
	inline static bool isBitOne(char byte, char position)
	{
		// bit 0 is the left most
		if (position < 0 || position > 7)
			return false;
		return byte & (1 << (7 - position));
	};

	inline static void setBit(char* byte, char position, char value)
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
	};
};

#endif /* BYTEOPERATORS_H_ */
