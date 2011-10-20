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
#include "OverflowParameter.h"

class LeafNode: public Node
{
private:
	int calculateMinimumSize();
	int calculateMaximumSize();
public:
	LeafNode(TreeBlock* tb, RecordMethods* methods);
	virtual OpResult insert(VariableRecord* keyRecord, VariableRecord* dataRecord, OverflowParameter& overflowParameter);
	virtual OpResult update(char* key, VariableRecord* r, OverflowParameter& overflowParameter);
	virtual OpResult remove(char* key);
	virtual bool get(char* key, VariableRecord* record);
	virtual VariableRecord* returnFirst(VariableRecord* r);
	virtual int getMaxSize();
	virtual int getMinimumSize();
	virtual bool isUnderflow();
	int getNextNode();
	VariableRecord* popFirst();
	VariableRecord* popLast();
	void print();
	virtual ~LeafNode();
};

#endif /* LEAFNODE_H_ */
