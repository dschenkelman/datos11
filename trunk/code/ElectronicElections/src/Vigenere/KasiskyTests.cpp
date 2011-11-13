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
	//this->separateCryptogramByKeyTest();
	this->getFrequenciesTest();
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
	//string message("6LSOGgYmOJl(dLRNN]Qv.^MJ`QYK^]c^Ug]mYW]a*u}uc z%v!vu|r)rz7NaWY!m7NaWYe#ucdRlVb%c}z?H]LI]UvKTi*ZH[JXZSl3jLbTISQlLm9W]a*uyiu$n)uz{s}r%r;TXbD2e;TXbDvvyi[]WgZ)iv*YUPOT`a\Lm0QSF[P^Yc7QlL]OJ\W]eSPcADfe<LWWQgqmzvyr(r!yt}n%3X^YO{v3X^YOa+qmaTbRk!mz{");
//	string message(";48B$2e?]JaL\L]_Jl1jVmzxxa>,23&(a)xzztys(v ucZLk[P%c:Lk[Pit1$F+8/&B22e?]JaL\L]_Jl1jVmzxxaEHaVc1KYZT/.A7J0C:}l$ZLaOJSQN6C:8(a-z348B5A;>%c/U\T^]JD2L6B%c#t<0B?777G!m-ZbO]YE:9=62e%|)76L98?4(a;HV`Fa9G;>>}lw++8>9@,L6)i(OOYIP^Fa9G;>>}lu,+8>9@,L6)i(OQYUSLNUXY=>?4A{vz$/.A7J0C:}l&`HaWJaWgUE:9=62e'{)76L98?4(a;SP]J[RjLE:9=62e$y)76L98?4(a;V[`RPXk=>?4A{v{}/.A7J0C:}l'Y`cZSD2L6B%c\"w<0B?777G!m/Z_X]Z]P;=7G:)iw#'A:C=.B22e3`WVDe=>?4A{vz /.A7J0C:}l(^MXYLVDe=>?4A{vx'/.A7J0C:}l)YQP]I]9G;>>}lv++8>9@,L6)i,cW`YXP;=7G:)iw''A:C=.B22e7LWbIgYSA4B2K!mzs 'A:C=.B22e7`SbLfNcZSl3YYZA4B2K!m}s2,K;A49={v1PNPaRf]XWQS9G;>>}lv)+8>9@,L6)i0SDjUbA4B2K!m}z2,K;A49={v3Pi5cHf[TA4B2K!m|v2,K;A49={v3X_YZHv9^NPD2L6B%c#y<0B?777G!m7T\JnPTb;=7G:)ix|'A:C=.B22e;`KYLf=>?4A{v{}/.A7J0C:}l0]U^XT\H]e5LQZVN6C:8(a.{348B5A;>%c;LYTXA4B2K!m}u2,K;A49={v4XOIZHlVfY;=7G:)iw%'A:C=.B22e<TIZDfKE:9=62e$ )76L98?4(aEV]_J`Hqe?LWY9G;>>}lv-+8>9@,L6)i3]UoH[V;=7G:)ix#'A:C=.B22e?PJYVcP[W;=7G:)ix#'A:C=.B22e?ZR]QY=>?4A{vz /.A7J0C:}l3gYci&`W`\aA4B2K!m}|2,K;A49={v7^]Yl+mY^Y;=7G:)iw%'A:C=.B22e?]TdRN6C:8(a-u348B5A;>%c?XaURd;=7G:)ix\"'A:C=.B22eASN\HdH]OJ`9G;>>}lu)+8>9@,L6)i7]F`Lb_J`9G;>>}lv/+8>9@,L6)i7gHN6C:8(a-x348B5A;>%cADaWPY;=7G:)iw!'A:C=.B22eBLSl$fNTWTD2L6B%c#z<0B?777G!m>F\WYeAZXO9G;>>}lv,+8>9@,L6)i8QDjI^]TcJ`=>?4A{v|}/.A7J0C:}l6]Hc_QS9G;>>}lu(+8>9@,L6)i8bov.TZWUHN6C:8(a,x348B5A;>%cAWYYZaNZO]=>?4A{vz(/.A7J0C:}l7Y\]_T\9G;>>}lu1+8>9@,L6)i9SPhSTi(WWq=>?4A{vy%/.A7J0C:}l:Y[T]GcUq=>?4A{vx'/.A7J0C:}l:Y`R]TaVN6C:8(a-v348B5A;>%cERgUbZHYHl=>?4A{vy\"/.A7J0C:}l%dHRVXPXjNE:9=62e })76L98?4(a;V]NT`GN6C:8(a*|348B5A;>%c2HlY^TYD2L6B%c''A:C=.B22eBLQWQYZE:9=62e!z)76L98?4(aOLb_cEDj^XNPD2L6B%c}r");
//	Kasisky k;
//	k.determineRepeatedNgrams(message,3);
//	k.calculateDistances(message, 3);
//	vector<int> keyLengthVector = k.estimateKeyLength();
//	cout << endl << "Estimated Key Length (in descending order of probability): ";
//	for (unsigned int i = 0; i<keyLengthVector.size(); i++)
//	{
//		cout << keyLengthVector[i] << " - ";
//	}
}

void KasiskyTests::separateCryptogramByKeyTest()
{
//	string message("6LSOGgYmOJl(dLRNN]Qv.^MJ`QYK^]c^Ug]mYW]a*u}uc z%v!vu|r)rz7NaWY!m7NaWYe#ucdRlVb%c}z?H]LI]UvKTi*ZH[JXZSl3jLbTISQlLm9W]a*uyiu$n)uz{s}r%r;TXbD2e;TXbDvvyi[]WgZ)iv*YUPOT`a\Lm0QSF[P^Yc7QlL]OJ\W]eSPcADfe<LWWQgqmzvyr(r!yt}n%3X^YO{v3X^YOa+qmaTbRk!mz{");
//	Kasisky k;
//	k.determineRepeatedNgrams(message,3);
//	k.calculateDistances(message, 3);
//	vector<int> keyLengthVector = k.estimateKeyLength();
//	int moreLikely = keyLengthVector[0];
//	cout << endl << "Estimated Key Length: " << moreLikely;
//
//	vector<string> separatedCryptogram = k.separateCryptogramByKey(message,moreLikely);
//	for (unsigned int i = 0; i<separatedCryptogram.size(); i++)
//	{
//		cout << endl << "Position Key: " << i << endl << separatedCryptogram[i] << endl;
//	}

}

void KasiskyTests::getFrequenciesTest()
{
	//string message("6LSOGgYmOJl(dLRNN]Qv.^MJ`QYK^]c^Ug]mYW]a*u}uc z%v!vu|r)rz7NaWY!m7NaWYe#ucdRlVb%c}z?H]LI]UvKTi*ZH[JXZSl3jLbTISQlLm9W]a*uyiu$n)uz{s}r%r;TXbD2e;TXbDvvyi[]WgZ)iv*YUPOT`a\Lm0QSF[P^Yc7QlL]OJ\W]eSPcADfe<LWWQgqmzvyr(r!yt}n%3X^YO{v3X^YOa+qmaTbRk!mz{");
	string message(";48B$2e?=*A,<,=?*l1J6mzxl)=*7,}lr+r zp q)vm7.A79!m7NaWYe .&<'A+0?4(aH94>.2(F;4i3@2vv$i2/5Ce23&A(<0B?777G!m,'35<,49;=7G:)ix#'A:C=.B22e0=);2J,E:9=62e$|)76L98?4(a:<C7*@9G;>>}lw++8>9@,L6)i(/*M(BA4B2K!m!v2,K;A49={v*07&0$K(BA4B2K!m}w2,K;A49={v*09&<'906@&D2L6B%c#v<0B?777G!m.-/5D,B?4<9G;>>}ly*+8>9@,L6)i(:$J,<:739G;>>}lv(+8>9@,L6)i(=/M41@8D2L6B%c$q<0B?777G!m/&G7G5E:9=62e#!)76L98?4(a<<@@*A1==>?4A{vy$/.A7J0C:}l'M97,2D2L6B%c#r<0B?777G!m0+4,F.7,2D2L6B%c!y<0B?777G!m1&8$J+>A4B2K!m v2,K;A49={v.D?-@,==>?4A{vy(/.A7J0C:}l+99C14@'N6C:8(a)u!/.A7J0C:}l+M5C4357G5m;&@.N6C:8(a,u348B5A;>%c8$;2B:3D,D34A4B2K!m t2,K;A49={v24,7<6N6C:8(a,|348B5A;>%c:$v7D03B(N6C:8(a+x348B5A;>%c:,L;;0c@2;2E:9=62e$#)76L98?4(aD6=2;7(O=>?4A{vz}/.A7J0C:}l/M-:43D2L6B%c$q<0B?777G!m8*<2E6=0*l)93;>;=7G:)iy$'A:C=.B22e<4&;,N6C:8(a,w348B5A;>%c;,<+;09=:F=>?4A{vy&/.A7J0C:}l0A+;,329G;>>}lv-+8>9@,L6)i2=1L,A0>l399:A4B2K!m x2,K;A49={v5>=<//C=>?4A{vz$/.A7J0C:}l3=,:>07/D=>?4A{vz$/.A7J0C:}l3G4>9&D2L6B%c#r<0B?777G!m;4@7v(A?-C5N6C:8(a, 348B5A;>%c>2J;m3:@2F=>?4A{vy&/.A7J0C:}l3J6E:;=7G:)ix|'A:C=.B22e@@.<&Q=>?4A{vz#/.A7J0C:}l5@0=01/1<,AA4B2K!m}t2,K;A49={v9>.-36L,AA4B2K!m z2,K;A49={v9H0;=7G:)ix!'A:C=.B22eB,.>$F=>?4A{vy\"/.A7J0C:}l695m,35(D6E:9=62e$$)76L98?4(aK(=?&l5G:0A4B2K!m w2,K;A49={v:2,702J6D2-D2L6B%c%q<0B?777G!m>*/7L34A4B2K!m}s2,K;A49={v:Cwc5(G960;=7G:)iw!'A:C=.B22eB?&@.N0;7*D2L6B%c#z<0B?777G!m?&C1L6=A4B2K!m}|2,K;A49={v;485:(v*8?>D2L6B%c\"w<0B?777G!mB&B(J)D=>D2L6B%c!y<0B?777G!mB&G&J6B>;=7G:)ix}'A:C=.B22eF:4<6G*:09D2L6B%c\"t<0B?777G!m-1/&C:1@759G;>>}lr,+8>9@,L6)i(=1;6A/;=7G:)iu%'A:C=.B22e309@2A;E:9=62e(/.A7J0C:}l69389&A9G;>>}ls)+8>9@,L6)i<36LeF,7E,;2E:9=62e z;=7G:m?4B$D,B%c x(w");
	Kasisky k;
	k.attack(message,3);
	/*map<char,double> frequencies = k.getFrequencies(message);
	map<char,double>::iterator it = frequencies.begin();
	cout << "Message: " << message << endl << endl;
	while (it != frequencies.end())
	{
		cout << "char: " << (*it).first << "\tfrequency: " << (*it).second << endl;
		it++;
	}
	float sumaTotal = 0.0;
	for (it = frequencies.begin(); it != frequencies.end(); it++)
	{
		sumaTotal += (*it).second;
	}
	cout<< endl << "SUMA TOTAL:" << sumaTotal << endl;*/
}

KasiskyTests::~KasiskyTests()
{
}
