/*
 * LeafNode.h
 *
 *  Created on: 20/09/2011
 *      Author: gonzalo
 */

#ifndef LEAFNODE_H_
#define LEAFNODE_H_

#include "Node.h"
#include "TreeBlock.h"

class LeafNode: public Node
{
private:
	int minimumRecords;
	int recordCount;
	int getMinimumRecords();
public:
	LeafNode(TreeBlock* tb, RecordMethods* methods);
	OpResult insert(char* key, Record* r);
	void print();
	virtual ~LeafNode();
};

#endif /* LEAFNODE_H_ */
