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
	static const int CANDIDATE_KEYS = 3;
	std::map<std::string, std::vector<int> > repeatedNgrams;
	std::vector<int> distances;
	std::vector<int> estimatedKeyLengths;
	std::vector<std::string> cryptogramsByKey;
	char getCharacterByFrequency(double frequency);
public:
	void calculateDistances(std::string& message, int nGramLength);
	void determineRepeatedNgrams(std::string& message, int nGramLength);
	std::map<std::string, std::vector<int> > getRepeatedNgrams();
	void estimateKeyLength();
	void separateCryptogramByKey(std::string message, int keyLength);
	std::map<char, double> getFrequencies (std::string cryptogram);
	void attack(std::string& message, int nGram);
	Kasisky();
	virtual ~Kasisky();
};

#endif /* KASISKY_H_ */
