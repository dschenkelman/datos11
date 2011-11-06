/*
 * PrimeGenerator.cpp
 *
 *  Created on: 06/11/2011
 *      Author: alejandro
 */

#include "PrimeGenerator.h"
#include <cstdlib>
#include <time.h>

PrimeGenerator::PrimeGenerator(int end) {
	srand( time(NULL) );
	// two first values are not used for simplicity
	this->end = end;
	primes = new bool[end+1];
	// initial setup: all true
	for (int i=2;i<=end;i++) {
		primes[i] = true;
	}
	this->total = end-1;
	// starting algorithm
	for (int i=2;i<=end/2;i++) {
		if (primes[i] == true) {
			for (int j=2*i;j<=end;j+=i) {
				if (primes[j]) this->total--;
				primes[j] = false;
			}
		}
	}
//	for (int i=0;i<end;i++) {
//		primes[i]? cout << "true"<<endl : cout << "false"<<endl;
//	}
}

int PrimeGenerator::totalGenerated() {
	return this->total;
}

void PrimeGenerator::getAllPrimes(int* res) {
	int position = 0;
	for (int i=2;i<=end;i++) {
		if (primes[i]) {
			res[position++] = i;
		}
	}
}

int PrimeGenerator::getRandomWithMinimum(int minimum) {
	int total = totalGenerated();
	int* all = new int[total];
	getAllPrimes(all);
	if (all[total-1]<minimum) return -1;
	int res;
	do {
		int num = rand()%total;
		res = all[num];
	} while (res<minimum);
	delete all;
	return res;
}

PrimeGenerator::~PrimeGenerator() {
	delete primes;
}
