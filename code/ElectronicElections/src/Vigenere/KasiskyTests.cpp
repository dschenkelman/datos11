/*
 * KasiskyTests.cpp
 *
 *  Created on: Nov 6, 2011
 *      Author: juanma
 */

#include "KasiskyTests.h"
#include "Kasisky.h"
#include <string>
#include <map>
#include <iostream>

using namespace std;

KasiskyTests::KasiskyTests()
{
}

void KasiskyTests::run()
{
	//this->determineRepeatedNgramsTest();
	//this->estimateKeyLengthTest();
	this->separateCryptogramByKeyTest();
}

void KasiskyTests::determineRepeatedNgramsTest()
{
	string message("QCIWYWOCMGWSBULGRLBFVRLSNPIKPWDCEZIKUCVRGGORFXTWSCVRDGSFFKBYMODURGNSMUCSNCIOVVFVRSFJFSEZSFDSJKPUPBUUVDFZCKZGIOJZEWMQZKPGBZCGpSHSEZIKFDIKKMOHRHEIVSRTMEBZVYIKUSVRDGSFFVIJNOEKGAPSEARJJBTURWTHRH");
	Kasisky k;
	k.determineRepeatedNgrams(message,3);
	std::map<std::string, std::vector<int> > repeatedNgrams;
	map<string, vector<int> >::iterator it;

	repeatedNgrams = k.getRepeatedNgrams();

	for ( it=repeatedNgrams.begin() ; it != repeatedNgrams.end(); it++ )
	{
		cout << (*it).first << " => ";
		for (unsigned  int i = 0; i < (*it).second.size(); i++)
		{
			cout << (*it).second[i] << ", ";
		}
		cout << endl;
	}
}

void KasiskyTests::estimateKeyLengthTest()
{
	string message("6LSOGgYmOJl(dLRNN]Qv.^MJ`QYK^]c^Ug]mYW]a*u}uc z%v!vu|r)rz7NaWY!m7NaWYe#ucdRlVb%c}z?H]LI]UvKTi*ZH[JXZSl3jLbTISQlLm9W]a*uyiu$n)uz{s}r%r;TXbD2e;TXbDvvyi[]WgZ)iv*YUPOT`a\Lm0QSF[P^Yc7QlL]OJ\W]eSPcADfe<LWWQgqmzvyr(r!yt}n%3X^YO{v3X^YOa+qmaTbRk!mz{");
	Kasisky k;
	k.determineRepeatedNgrams(message,3);
	k.calculateDistances(message, 3);
	vector<int> keyLengthVector = k.estimateKeyLength();
	cout << endl << "Estimated Key Length (in descending order of probability): ";
	for (unsigned int i = 0; i<keyLengthVector.size(); i++)
	{
		cout << keyLengthVector[i] << " - ";
	}
}

void KasiskyTests::separateCryptogramByKeyTest()
{
	string message("6LSOGgYmOJl(dLRNN]Qv.^MJ`QYK^]c^Ug]mYW]a*u}uc z%v!vu|r)rz7NaWY!m7NaWYe#ucdRlVb%c}z?H]LI]UvKTi*ZH[JXZSl3jLbTISQlLm9W]a*uyiu$n)uz{s}r%r;TXbD2e;TXbDvvyi[]WgZ)iv*YUPOT`a\Lm0QSF[P^Yc7QlL]OJ\W]eSPcADfe<LWWQgqmzvyr(r!yt}n%3X^YO{v3X^YOa+qmaTbRk!mz{");
	Kasisky k;
	k.determineRepeatedNgrams(message,3);
	k.calculateDistances(message, 3);
	vector<int> keyLengthVector = k.estimateKeyLength();
	int moreLikely = keyLengthVector[0];
	cout << endl << "Estimated Key Length: " << moreLikely;

	vector<string> separatedCryptogram = k.separateCryptogramByKey(message,moreLikely);
	for (unsigned int i = 0; i<separatedCryptogram.size(); i++)
	{
		cout << endl << "-" << i << endl << separatedCryptogram[i] << endl;
	}

}

KasiskyTests::~KasiskyTests()
{
}
