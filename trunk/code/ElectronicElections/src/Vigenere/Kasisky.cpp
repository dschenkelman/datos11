/*
 * Kasisky.cpp
 *
 *  Created on: Nov 6, 2011
 *      Author: juanma
 */

#include "Kasisky.h"
#include <string>
#include <string.h>
#include <iostream>

using namespace std;

Kasisky::Kasisky()
{
}

void Kasisky::attack(string& message, int nGramLength)
{
}

void Kasisky::determineRepeatedNgrams(string& message, int nGramLength)
{
	map<string,vector<int> >::iterator it;
	vector<map<string,vector<int> >::iterator> nonRepeated;
	for(unsigned int i = 0; i < message.length()-(nGramLength-1); i++ )
	{
		string nGram = message.substr(i,nGramLength);

		it = repeatedNgrams.find(nGram);
		if (it == repeatedNgrams.end())
		{
			// The nGram was not found before
			this->repeatedNgrams[nGram] = vector<int>();
		}

		this->repeatedNgrams[nGram].push_back(i);

	}

	for ( it=repeatedNgrams.begin() ; it != repeatedNgrams.end(); it++ )
	{
		if ((*it).second.size() == 1)
		{
			nonRepeated.push_back(it);
		}
	}
	while (!nonRepeated.empty())
	{
		repeatedNgrams.erase(nonRepeated.back());
		nonRepeated.pop_back();
	}

}

void Kasisky::calculateDistances(string& message, int nGramLength)
{
	map<string, vector<int> >::iterator it;

	// go through the list of repeated ngrams and do distance calculations, etc
	for ( it=this->repeatedNgrams.begin() ; it != this->repeatedNgrams.end(); it++ )
	{
		string nGram = it->first;
		vector<int>& indexes = it->second;

		map<string, vector<int> > distances;

		// calculate ngrams distance
		for(unsigned int i = 0; i < indexes.size(); i++)
		{
			for(unsigned int j = j+1; i < indexes.size(); j++ )
			{
				distances[nGram].push_back(indexes[j] - indexes[i]);
			}
		}
	}
}

void Kasisky::estimateKeyLength()
{
}

std::map<std::string,std::vector<int> > Kasisky::getRepeatedNgrams()
{
	return this->repeatedNgrams;
}

Kasisky::~Kasisky()
{
}
