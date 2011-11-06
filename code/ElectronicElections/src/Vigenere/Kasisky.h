/*
 * Kasisky.h
 *
 *  Created on: Nov 6, 2011
 *      Author: juanma
 */

#ifndef KASISKY_H_
#define KASISKY_H_
#include <string>
#include <vector>

using namespace std;

class Kasisky
{
	vector<string> repeatedNgrams;

	void determineRepeatedNgrams(string message, int nGram);
	void calculateDistances(string message, int nGram);
	void estimateKeyLength();
public:
	Kasisky();
	void attack(string message, int nGram);

	virtual ~Kasisky();
};

#endif /* KASISKY_H_ */
