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
#include <algorithm>
#include <math.h>

using namespace std;

Kasisky::Kasisky()
{
}

void Kasisky::attack(string& message, int nGramLength)
{
	this->determineRepeatedNgrams(message,nGramLength);
	this->calculateDistances(message, nGramLength);
	vector<int> keyLength = this->estimateKeyLength();
	int moreLikely = keyLength[0];
	vector<string> cryptogramByKey = this->separateCryptogramByKey(message,moreLikely);
	vector<string> messageByKey;

	for (unsigned int i = 0; i < cryptogramByKey.size(); i++)
	{
		map<char,double> frequencies = this->getFrequencies(cryptogramByKey[i]);
		map<char,double>::iterator it;
		map<char,char> dictionary;	// map < crypted , decrypted >
		string subMessage;
		for (it = frequencies.begin(); it != frequencies.end(); it++)
		{
			dictionary[(*it).first] = this->getCharacterByFrequency((*it).second);
			//dictionary[(*it).first] = 'f';
		}
		for (unsigned int j = 0; j < cryptogramByKey[i].size(); j++)
		{
			subMessage.push_back(dictionary[cryptogramByKey[i][j]]);
		}
		messageByKey.push_back(subMessage);
	}

	string decryptedMessage;
	for (unsigned int i = 0; i < messageByKey[0].size();i++)
	{
		for (int j = 0; j < moreLikely && j < messageByKey[j].size(); j++)
		{
			decryptedMessage.push_back(messageByKey[j][i]);
		}
	}
	cout << endl << "Decrypted: " << decryptedMessage;
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
	map<string, vector<int> > nGramDistances;
	// go through the list of repeated ngrams and do distance calculations, etc
	for (it=this->repeatedNgrams.begin() ; it != this->repeatedNgrams.end(); it++ )
	{
		string nGram = it->first;
		cout << "nGram:" << nGram << " . distances:";
		nGramDistances[nGram] = vector<int>();


		   vector<int> indexes = it->second;

		   // calculate ngrams distance
		   for(unsigned int i = 0; i < indexes.size(); i++)
		   {
				   for(unsigned int j = i+1; j < indexes.size(); j++ )
				   {
						   cout << indexes[j] - indexes[i] << ", ";
						   nGramDistances[nGram].push_back(indexes[j] - indexes[i]);
				   }
		   }

		   cout << endl;
	}
	for (it = nGramDistances.begin(); it != nGramDistances.end(); it++)
	{
		this->distances.insert(this->distances.end(),(*it).second.begin(),(*it).second.end());
	}
}

char Kasisky::getCharacterByFrequency(double frequency)
{
	const LetterFrequencyPair frequencies[] = {{'e', 13.68},{'a', 12.53},
					{'o', 8.68},{'s', 7.98},{'r', 6.87},{'n', 6.71},{'i', 6.25},
					{'d', 5.86},{'l', 4.97},{'c', 4.68},{'t', 4.63},{'u', 3.93},
					{'m', 3.15},{'p', 2.51},{'b', 1.42},{'g', 1.01},{'v', 0.9},
					{'y', 0.9},{'q', 0.88},{'h', 0.7},{'f', 0.69},{'z', 0.52},
					{'j', 0.44},{'x', 0.22},{'w', 0.02},{'k', 0.01}
			};

	LetterFrequencyPair pair = frequencies[0];
	for (int i=0; i < 26; i++)
	{
		if (frequencies[i].frequency > frequency)
		{
			pair = frequencies[i];
		}
	}

	return pair.letter;
}

vector<int> Kasisky::estimateKeyLength()
{
	int numberOfCandidates = 3;
	map<int, int> frequencyOfDistances;
	map<int, int>::iterator it;
	for (unsigned int i = 0; i < this->distances.size(); i++)
	{
		int distance = this->distances[i];
		if (frequencyOfDistances.count(distance) <= 0)	//The distance does not exists
		{
			frequencyOfDistances[distance] = 0;
		}
		frequencyOfDistances[distance]++;
		for (unsigned int j = 2; j <= sqrt(distance); j++)
		{
			if (distance % j == 0)

			{
				if (frequencyOfDistances.count(j) <= 0)
				{
					frequencyOfDistances[j] = 0;
				}
				if (frequencyOfDistances.count(distance/j) <= 0)
				{
					frequencyOfDistances[distance/j] = 0;
				}
				frequencyOfDistances[j]++;
				frequencyOfDistances[distance/j]++;
			}
		}
	}

	for ( it=frequencyOfDistances.begin() ; it != frequencyOfDistances.end(); it++ )
	{
		cout<< endl << "Distance: " << (*it).first << "\tCount: " << (*it).second;
	}

	// Get More Frequent Distance
	vector<int> moreFrequentDistances;

	for (int i = 0; i < numberOfCandidates && !frequencyOfDistances.empty(); i++)
	{
		int moreFrequentDistance = 0;
		int maxValue = 0;

		for ( it=frequencyOfDistances.begin() ; it != frequencyOfDistances.end(); it++ )
		{
			if (((*it).second) > maxValue)
			{
				maxValue = (*it).second;
				moreFrequentDistance = (*it).first;
			}
		}
		moreFrequentDistances.push_back(moreFrequentDistance);
		frequencyOfDistances.erase(moreFrequentDistance);
	}

	return moreFrequentDistances;
}

vector<string> Kasisky::separateCryptogramByKey(string message, int keyLength)
{
	vector<string> cryptogramByKey;
	for (int i = 0; i < keyLength; i++)
	{
		int j = i;
		string subMessage;
		while (j < message.size() )
		{
			subMessage += message[j];
			j += keyLength;
		}
		cryptogramByKey.push_back(subMessage);
	}
	return cryptogramByKey;
}

map<string,vector<int> > Kasisky::getRepeatedNgrams()
{
	return this->repeatedNgrams;
}

map<char, double> Kasisky::getFrequencies (string cryptogram)
{
	map<char, double> frequencies;
	map<char, double>::iterator it;
	for (unsigned int i = 0; i < cryptogram.size(); i++)
	{
		frequencies[cryptogram[i]]++;
	}
	for (it = frequencies.begin(); it != frequencies.end(); it++)
	{
		frequencies[(*it).first] = (*it).second * 100 / cryptogram.size();
	}
	cout<<endl;
	cout<<"crypto: " << cryptogram << endl;
	for (it = frequencies.begin(); it != frequencies.end(); it++)
	{
		cout  << "char: " << (*it).first << "\tfreq: " << (*it).second << endl;
	}
	return frequencies;
}


/*
vector<int> Kasisky::trialDivision(int number)
{
	vector<int> prime_numbers;
	int prime;

	// Generate Prime numbers
	vector<int> allPrimeNumbers;
	if (number <= 1)
	{
		cerr<<"No se puede factorizar un numero menor o igual a 1";
		return prime_numbers;
	}

	for (unsigned int i = 0; i < allPrimeNumbers.size(); i++)
	{
		prime = allPrimeNumbers[i];
		if (prime*prime > number)
			break;
		while (number % prime == 0)
		{
			prime_numbers.push_back(number);
			number = number / prime;
		}
	}
	// Let's not include the same number
	//prime_numbers.push_back(number);

}
*/

Kasisky::~Kasisky()
{
}
