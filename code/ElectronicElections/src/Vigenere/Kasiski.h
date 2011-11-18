/*
 * Kasisky.h
 *
 *  Created on: Nov 6, 2011
 *      Author: juanma
 */

#ifndef KASISKI_H_
#define KASISKI_H_
#include <string>
#include <map>
#include <vector>
#include <map>
#include "LetterFrequencyPair.h"

class Kasiski
{
	typedef unsigned long long uInt64;
	static const int CANDIDATE_KEYS = 3;
	std::map<std::string, std::vector<int> > repeatedNgrams;
	std::vector<int> distances;
	std::vector<int> estimatedKeyLengths;
	std::vector<std::string> cryptogramsByKey;
	char getKeyLetterConsideringAsCharacter(char mostFrequent, char letter);
public:
	void calculateDistances(int nGramLength);
	void determineRepeatedNgrams(std::string& message, int nGramLength);
	std::map<std::string, std::vector<int> > getRepeatedNgrams();
	void estimateKeyLength();
	void separateCryptogramByKey(std::string message, int keyLength);
	std::map<char, double> getFrequencies (std::string cryptogram);
	void attack(std::string& message, int nGram);
	Kasiski();
	virtual ~Kasiski();
};

#endif /* KASISKI_H_ */
