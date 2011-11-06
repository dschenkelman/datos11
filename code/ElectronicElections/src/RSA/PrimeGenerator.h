/*
 * PrimeGenerator.h
 *
 *  Created on: 06/11/2011
 *      Author: alejandro
 */

#ifndef PRIMEGENERATOR_H_
#define PRIMEGENERATOR_H_

#include <iostream>
using namespace std;

class PrimeGenerator {
private:
	bool* primes;
	int end;
	int total;
public:
	PrimeGenerator(int end);
	int totalGenerated();
	void getAllPrimes(int* res);
	int getRandomWithMinimum(int minimum);
	virtual ~PrimeGenerator();
};

#endif /* PRIMEGENERATOR_H_ */
