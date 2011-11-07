/*
 * KasiskyTests.cpp
 *
 *  Created on: Nov 6, 2011
 *      Author: juanma
 */

#include "KasiskyTests.h"
#include "Kasisky.h"
#include <string>

using namespace std;

KasiskyTests::KasiskyTests() {
	// TODO Auto-generated constructor stub

}

void KasiskyTests::run()
{
	string message("QCIWYWOCMGWSBULGRLBFVRLSNPIKPWDCEZIKUCVRGGORFXTWSCVRDGSFFKBYMODURGNSMUCSNCIOVVFVRSFJFSEZSFDSJKPUPBUUVDFZCKZGIOJZEWMQZKPGBZCGpSHSEZIKFDIKKMOHRHEIVSRTMEBZVYIKUSVRDGSFFVIJNOEKGAPSEARJJBTURWTHRH");
	Kasisky k;
	k.determineRepeatedNgrams(message,3);
}

KasiskyTests::~KasiskyTests() {
	// TODO Auto-generated destructor stub
}
