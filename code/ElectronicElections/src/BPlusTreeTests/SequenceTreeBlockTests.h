/*
 * SequenceTreeBlockTests.h
 *
 *  Created on: 08/10/2011
 *      Author: JuanMa
 */

#ifndef SEQUENCETREEBLOCKTESTS_H_
#define SEQUENCETREEBLOCKTESTS_H_
#include "../BPlusTree/SequenceTreeBlock.h"
#include "../Entities/DistrictMethods.h"
#include <string>

class SequenceTreeBlockTests {
	void printResult(std::string testName, bool result);
	DistrictMethods districtMethods;
		SequenceTreeBlock* block1;
public:
	SequenceTreeBlockTests();
	bool testPopFirstWorksCorrectly();
	void print();
	void run();
	virtual ~SequenceTreeBlockTests();
};

#endif /* SEQUENCETREEBLOCKTESTS_H_ */
