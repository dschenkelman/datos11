/*
 * RSAKey.h
 *
 *  Created on: 06/11/2011
 *      Author: gonzalo
 */

#ifndef RSAKEY_H_
#define RSAKEY_H_

typedef long long int64;
typedef unsigned long long uInt64;

struct RSAKey
{
	int64 n;
	int64 exp;
};


#endif /* RSAKEY_H_ */
