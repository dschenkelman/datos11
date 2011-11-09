/*
 * PrimeGenerator.h
 *
 *  Created on: 06/11/2011
 *      Author: alejandro
 */

#ifndef PRIMEGENERATOR_H_
#define PRIMEGENERATOR_H_

#include <iostream>
#include "RSAKey.h"
#include <vector>

class PrimeGenerator {
private:
	int end;
	int total;
	std::vector<int> primeNumbers;
public:
	PrimeGenerator(int end);
	int getRandomWithMinimum(int minimum, int64* p, int64* q);
	virtual ~PrimeGenerator();
};

#endif /* PRIMEGENERATOR_H_ */
