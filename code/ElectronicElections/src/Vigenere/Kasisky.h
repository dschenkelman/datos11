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

class Kasisky
{
	std::map<std::string, std::vector<int> > repeatedNgrams;

	void calculateDistances(std::string& message, int nGramLength);
	void estimateKeyLength();
public:
	void determineRepeatedNgrams(std::string& message, int nGramLength);
	Kasisky();
	void attack(std::string& message, int nGram);

	virtual ~Kasisky();
};

#endif /* KASISKY_H_ */
