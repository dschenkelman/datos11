/*
 * Node.h
 *
 *  Created on: 20/09/2011
 *      Author: gonzalo
 */

#ifndef NODE_H_
#define NODE_H_
#include "../VariableBlocks/RecordMethods.h"
#include "TreeBlock.h"
#include "OverflowParameter.h"

enum OpResult
{
		Unchanged,
		Updated,
        Overflow,
        Underflow,
        NotFound,
        Duplicated
};

class Node
{
protected:
	TreeBlock* block;
	RecordMethods* recordMethods;
public:
	Node(TreeBlock* b, RecordMethods* methods);
	virtual OpResult insert(VariableRecord* keyRecord, VariableRecord* dataRecord, OverflowParameter& overflowParameter) = 0;
	virtual OpResult update(char* key, VariableRecord* r) = 0;
	virtual OpResult remove(char* key) = 0;
	virtual void print() = 0;
	virtual int getMaxSize() = 0;
	short getLevel();
	void increaseLevel();
	virtual ~Node();
};

#endif /* NODE_H_ */
