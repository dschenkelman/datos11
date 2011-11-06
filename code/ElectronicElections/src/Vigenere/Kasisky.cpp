/*
 * Kasisky.cpp
 *
 *  Created on: Nov 6, 2011
 *      Author: juanma
 */

#include "Kasisky.h"
#include <string>

using namespace std;

Kasisky::Kasisky()
{
}

void Kasisky::attack(string& message, int nGramLength)
{
}

void Kasisky::determineRepeatedNgrams(string& message, int nGramLength)
{
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

Kasisky::~Kasisky()
{
}
