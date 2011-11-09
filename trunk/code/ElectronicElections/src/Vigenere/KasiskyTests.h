/*
 * KasiskyTests.h
 *
 *  Created on: Nov 6, 2011
 *      Author: juanma
 */

#ifndef KASISKYTESTS_H_
#define KASISKYTESTS_H_

class KasiskyTests {
public:
	KasiskyTests();
	void estimateKeyLengthTest();
	void determineRepeatedNgramsTest();
	void run();
	virtual ~KasiskyTests();
};

#endif /* KASISKYTESTS_H_ */
