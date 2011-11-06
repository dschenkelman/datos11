/*
 * RSACipher.cpp
 *
 *  Created on: 06/11/2011
 *      Author: lejosdelcielo
 */

#include "RSACipher.h"

RSACipher::RSACipher() {
	// TODO Auto-generated constructor stub

}
#include <iostream>
using namespace std;
int64 RSACipher::productInverse(int64 n, int64 d) {
	int64 olda1 = 1;
	int64 olda2 = 0;
	int64 olda3 = n;
	int64 oldb1 = 0;
	int64 oldb2 = 1;
	int64 oldb3 = d;
	int64 a1,a2,a3,b1,b2,b3;
	int64 q;
	do {
		q = olda3/oldb3;
		a1 = oldb1;
		a2 = oldb2;
		a3 = oldb3;
		b1 = olda1 - q*oldb1;
		b2 = olda2 - q*oldb2;
		b3 = olda3 - q*oldb3;
		olda1 = a1;
		olda2 = a2;
		olda3 = a3;
		oldb1 = b1;
		oldb2 = b2;
		oldb3 = b3;
//		cout << " a1 " << a1 << " a2 " << a2 << " a3 " << a3 << " b1 " << b1 << " b2 " << b2 << " b3 " << b3 << endl;
	} while((b3!=1)&&(b3!=0));
	if (b3==1) return b2;
	return -1;
}

RSACipher::~RSACipher() {
	// TODO Auto-generated destructor stub
}

