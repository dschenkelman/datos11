/*
 * LeafNode.h
 *
 *  Created on: 20/09/2011
 *      Author: gonzalo
 */

#ifndef LEAFNODE_H_
#define LEAFNODE_H_

#include "Node.h"
#include "SequenceTreeBlock.h"
#include "../VariableBlocks/VariableRecord.h"

class LeafNode: public Node
{
private:
	SequenceTreeBlock* block;
	int minimumSize;
	int getMinimumSize();
public:
	LeafNode(SequenceTreeBlock* tb, RecordMethods* methods);
	OpResult insert(char* key, VariableRecord* r);
	void print();
	virtual ~LeafNode();
};

#endif /* LEAFNODE_H_ */
