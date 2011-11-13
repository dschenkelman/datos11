/*
 * Kasisky.h
 *
 *  Created on: Nov 6, 2011
 *      Author: juanma
 */

#ifndef KASISKY_H_
#define KASISKY_H_
#include <string>
#include <map>
#include <vector>
#include <map>
#include "LetterFrequencyPair.h"

class Kasisky
{
	std::map<std::string, std::vector<int> > repeatedNgrams;
	std::vector<int> distances;
	//std::vector<int> trialDivision(int number);

public:
	void calculateDistances(std::string& message, int nGramLength);
	void determineRepeatedNgrams(std::string& message, int nGramLength);
	std::map<std::string, std::vector<int> > getRepeatedNgrams();
	std::vector<int> estimateKeyLength();
	std::vector<std::string> separateCryptogramByKey(std::string message, int keyLength);
	void attack(std::string& message, int nGram);

	Kasisky();
	virtual ~Kasisky();
};

#endif /* KASISKY_H_ */
