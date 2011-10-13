/*
 * LeafNodeTests.h
 *
 *  Created on: Sep 23, 2011
 *      Author: damian
 */

#ifndef LEAFNODETESTS_H_
#define LEAFNODETESTS_H_

#include <string>
#include "../BPlusTree/LeafNode.h"

class LeafNodeTests
{
	void printResult(std::string testName, bool result);
	void insertDistrict(LeafNode* node, std::string key);
public:
	LeafNodeTests();
	bool testInsertLessThanFullSizeReturnsCorrectResult();
	bool testInsertDuplicatedRecordReturnsCorrectResult();
	bool testInsertRecordInFullBlockReturnsOverflow();
	bool testInsertingWithOverflowPutsMiddleRecordInPassedParameter();
	bool testUpdateNonExistentRecordReturnsNotFound();
	bool testUpdateShouldReturnOverflowIfRecordDoesNotFitNode();
	bool testUpdateShouldUpdateRecordAndReturnUpdated();
	bool testGetNonExistentRecordReturnsFalse();
	bool testGetShouldGetRecordAndReturnTrue();
	bool testDeleteReturnsNotFoundIfKeyIsNotPresent();
	bool testDeleteReturnsUnderflowIfOccupiedSizeIsLessThanMinimum();
	bool testDeleteReturnsUpdatedIfOccupiedSizeIsMoreThanMinimumAndRecordWasDeleted();
	void run();
	virtual ~LeafNodeTests();
};

#endif /* LEAFNODETESTS_H_ */
