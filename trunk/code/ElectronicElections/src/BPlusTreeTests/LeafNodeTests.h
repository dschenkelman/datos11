/*
 * LeafNodeTests.h
 *
 *  Created on: Sep 23, 2011
 *      Author: damian
 */

#ifndef LEAFNODETESTS_H_
#define LEAFNODETESTS_H_

#include <string>

class LeafNodeTests
{
	void printResult(std::string testName, bool result);
public:
	LeafNodeTests();
	bool testInsertLessThanFullSizeReturnsCorrectResult();
	bool testInsertDuplicatedRecordReturnsCorrectResult();
	bool testInsertRecordInFullBlockReturnsOverflow();
	bool testInsertingWithOverflowPutsMiddleRecordInPassedParameter();
	bool testUpdateNonExistentRecordReturnsNotFound();
	bool testUpdateShouldReturnOverflowIfRecordDoesNotFitNode();
	bool testUpdateShouldUpdateRecordAndReturnUpdated();
	bool testDeleteReturnsNotFoundIfKeyIsNotPresent();
	bool testDeleteReturnsUnderflowIfOccupiedSizeIsLessThanMinimum();
	bool testDeleteReturnsUpdatedIfOccupiedSizeIsMoreThanMinimumAndRecordWasDeleted();
	void run();
	virtual ~LeafNodeTests();
};

#endif /* LEAFNODETESTS_H_ */
