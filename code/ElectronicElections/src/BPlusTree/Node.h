/*
 * Node.h
 *
 *  Created on: 20/09/2011
 *      Author: gonzalo
 */

#ifndef NODE_H_
#define NODE_H_
#include "../Blocks/RecordMethods.h"
#include "TreeBlock.h"

enum OpResult
{
	Updated,
	Overflow,
	Underflow
};

class Node
{
protected:
	RecordMethods* recordMethods;
	TreeBlock* treeBlock;
public:
	Node(TreeBlock* tb, RecordMethods* methods);
	virtual ~Node();
};

#endif /* NODE_H_ */
