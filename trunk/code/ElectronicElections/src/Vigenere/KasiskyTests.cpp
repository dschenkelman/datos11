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

KasiskyTests::~KasiskyTests()
{
}
