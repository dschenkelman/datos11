//============================================================================
// Name        : ElectronicElections.cpp
// Author  : Datos11
// Version     :
// Copyright   : 
// Description : Trying block file out
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <string>
#include "BlocksTests/BlockFileTests.h"

using namespace std;

int main()
{
	srand (time(NULL));
	BlockFileTests tests;
	tests.run();
	return 0;
}
