/*
 * Kasisky.cpp
 *
 *  Created on: Nov 6, 2011
 *      Author: juanma
 */

#include "Kasiski.h"
#include "VigenereCipher.h"
#include <string>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

Kasiski::Kasiski()
{
}

void Kasiski::attack(string& message, int nGramLength)
{
	this->determineRepeatedNgrams(message,nGramLength);
	this->calculateDistances(nGramLength);
	this->estimateKeyLength();
	string proposedKey = "";

	vector<string> messageByKey;

	for (int k = 0; k < 3; k++)
	{
		int keyLength = this->estimatedKeyLengths[k];
		this->separateCryptogramByKey(message,keyLength);
		uInt64 keyCombinations = (uInt64)pow(2, keyLength);
		vector<string> keys;

		for (unsigned int i = 0; i < this->cryptogramsByKey.size(); i++)
		{
			map<char,double> frequencies = this->getFrequencies(this->cryptogramsByKey[i]);
			map<char,double>::iterator it;
			map<char,char> dictionary;	// map < crypted , decrypted >
			string subMessage;
			char mostFrequent = 'Z';
			double frequency = 0;
			// popMoreFrequent

			for (it = frequencies.begin(); it != frequencies.end(); it++)	//get More Frequent from cryptogram
			{
				if ((*it).second > frequency)
				{
					mostFrequent = (*it).first;
					frequency = (*it).second;
				}
			}

			char keyLetter1 = this->getKeyLetterConsideringAsCharacter(mostFrequent, 'O');
			char keyLetter2 = this->getKeyLetterConsideringAsCharacter(mostFrequent, 'T');
			for (uInt64 j = 0; j < keyCombinations; j++)
			{
				keys.push_back("");
			}

			for (uInt64 j = 0; j < keyCombinations; j++)
			{
				// of course this does work
				if (((j >> i) & 1) == 0)
				{
					keys[j].push_back(keyLetter1);
				}
				else
				{
					keys[j].push_back(keyLetter2);
				}
			}
		}

		VigenereCipher vg;
		cout << endl << "Using Key Length: " << keyLength;
		bool found = false;
		for (uInt64 j = 0; j < keyCombinations; j++)
		{
			string decrypted = vg.decript(message,keys[j]);
			if (decrypted.find("DISTRITO", 0) != decrypted.npos &&
					decrypted.find("VOTOS", 0) != decrypted.npos)
			{
				cout << endl << "Proposed Key:" << keys[j];
				cout << endl << "Decrypted: " << decrypted << endl;
				found = true;
			}
		}

		if (found)
		{
			return;
		}
	}
}

void Kasiski::determineRepeatedNgrams(string& message, int nGramLength)
{
	string copy(message);
	int length = copy.length();

	for(unsigned int i = 0; i < length; i++)
	{
		if (!isalpha(copy[i]))
		{
			copy.erase(i, 1);
			i--;
			length--;
		}
	}

	map<string,vector<int> >::iterator it;
	vector<map<string,vector<int> >::iterator> nonRepeated;
	for(unsigned int i = 0; i < copy.length()-(nGramLength-1); i++ )
	{
		string nGram = copy.substr(i,nGramLength);

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

void Kasiski::calculateDistances(int nGramLength)
{
	map<string, vector<int> >::iterator it;
	map<string, vector<int> > nGramDistances;
	// go through the list of repeated ngrams and do distance calculations, etc
	for (it=this->repeatedNgrams.begin() ; it != this->repeatedNgrams.end(); it++ )
	{
		string nGram = it->first;
		nGramDistances[nGram] = vector<int>();

	   vector<int> indexes = it->second;

	   // calculate ngrams distance
	   for(unsigned int i = 0; i < indexes.size(); i++)
	   {
			   for(unsigned int j = i+1; j < indexes.size(); j++ )
			   {
					   nGramDistances[nGram].push_back(indexes[j] - indexes[i]);
			   }
	   }
	}

	for (it = nGramDistances.begin(); it != nGramDistances.end(); it++)
	{
		this->distances.insert(this->distances.end(),(*it).second.begin(),(*it).second.end());
	}
}

void Kasiski::estimateKeyLength()
{
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

	for (int i = 0; i < Kasiski::CANDIDATE_KEYS && !frequencyOfDistances.empty(); i++)
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
		this->estimatedKeyLengths.push_back(moreFrequentDistance);
		frequencyOfDistances.erase(moreFrequentDistance);
	}
}

void Kasiski::separateCryptogramByKey(string message, int keyLength)
{
	for (int i = 0; i < keyLength; i++)
	{
		this->cryptogramsByKey.push_back("");
	}

	int cont = 0;

	for (unsigned int i = 0; i < message.size(); i++)
	{
		if (isalpha(message[i]))
		{
			this->cryptogramsByKey[cont % keyLength].push_back(message[i]);
			cont++;
		}
	}
}

map<string,vector<int> > Kasiski::getRepeatedNgrams()
{
	return this->repeatedNgrams;
}

map<char, double> Kasiski::getFrequencies (string cryptogram)
{
	map<char, double> frequencies;
	map<char, double>::iterator it;
	int letters = 0;
	for (unsigned int i = 0; i < cryptogram.size(); i++)
	{
		if (isalpha(cryptogram[i]))
		{
			char letter = cryptogram[i];
			it = frequencies.find(letter);
			if (it == frequencies.end())
			{
				frequencies[letter] = 0;
			}
			letters++;
			frequencies[letter]++;
		}
	}

	for (it = frequencies.begin(); it != frequencies.end(); it++)
	{
		frequencies[(*it).first] = (*it).second * 100.0 / letters;
	}

	return frequencies;
}

char Kasiski::getKeyLetterConsideringAsCharacter(char mostFrequent, char letter)
{
	char keyLetter = (mostFrequent - 'A') - letter;
	while (keyLetter < 'A')
	{
		keyLetter += ('Z' - 'A' + 1);
	}

	return keyLetter;
}

Kasiski::~Kasiski()
{
}
