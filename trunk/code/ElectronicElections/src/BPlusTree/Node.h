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

enum OpResult
{
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
	virtual ~Node();
};

#endif /* NODE_H_ */
