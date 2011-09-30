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
	int minimumSize;
	int getMinimumSize();
public:
	LeafNode(TreeBlock* tb, RecordMethods* methods);
	virtual OpResult insert(VariableRecord* keyRecord, VariableRecord* dataRecord);
	virtual OpResult update(char* key, VariableRecord* r);
	virtual OpResult remove(char* key);
	void print();
	virtual ~LeafNode();
};

#endif /* LEAFNODE_H_ */
