/*
 * RSACipherTests.h
 *
 *  Created on: 06/11/2011
 *      Author: lejosdelcielo
 */

#ifndef RSACIPHERTESTS_H_
#define RSACIPHERTESTS_H_

class RSACipherTests {
private:
	void printResult(std::string testName, bool result);
public:
	RSACipherTests();
	void run();
	bool testgenerateRelativelyPrimeNumbers();
	virtual ~RSACipherTests();
};

#endif /* RSACIPHERTESTS_H_ */
