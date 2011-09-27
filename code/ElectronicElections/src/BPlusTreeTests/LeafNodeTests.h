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
	bool testInsertarRegistroEnBloqueLlenoColocaRegistroDelMedioEnPuntero(); // mi ingles es nulo, que alguien lo traduzca!!
	bool testUpdateNonExistentRecordReturnsNotFound();
	bool testUpdateShouldReturnOverflowIfRecordDoesNotFitNode();
	void run();
	virtual ~LeafNodeTests();
};

#endif /* LEAFNODETESTS_H_ */
