/*
 * DebugAid.h
 *
 *  Created on: Sep 13, 2011
 *      Author: damian
 */

#ifndef DEBUGAID_H_
#define DEBUGAID_H_

template <class T>
class Debug {
public:
	static T eval(T value)
	{
		T v = value;
		return v;
	};
};

#endif /* DEBUGAID_H_ */
