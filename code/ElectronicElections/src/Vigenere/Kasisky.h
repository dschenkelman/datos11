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
	std::map<std::string, std::vector<int> > distances;
	void calculateDistances(std::string& message, int nGramLength);
	void estimateKeyLength();
public:
	void determineRepeatedNgrams(std::string& message, int nGramLength);
	std::map<std::string, std::vector<int> > getRepeatedNgrams();
	Kasisky();
	void attack(std::string& message, int nGram);

	virtual ~Kasisky();
};

#endif /* KASISKY_H_ */
