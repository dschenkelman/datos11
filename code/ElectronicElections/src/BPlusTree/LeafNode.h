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
	TreeBlock* treeBlock;
	int minimunNumberOfRecordsPerBlock;
	int currentRecords;
	int getMinimunNumberOfRecordsPerBlock();
public:
	LeafNode(TreeBlock* tb);
	virtual ~LeafNode();
};

#endif /* LEAFNODE_H_ */
