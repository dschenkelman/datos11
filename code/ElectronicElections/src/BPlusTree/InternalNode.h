/*
 * InternalNode.h
 *
 *  Created on: Sep 30, 2011
 *      Author: damian
 */

#ifndef INTERNALNODE_H_
#define INTERNALNODE_H_

#include "Node.h"
#include "TreeBlockFile.h"
#include "OverflowParameter.h"
#include "LeafNode.h"

class InternalNode: public Node
{
	TreeBlockFile* file;
	int maximumSize;
	int calculateMaxSize();
    OpResult handleLeafOverflow(VariableRecord* keyRecord, VariableRecord* dataRecord, OverflowParameter& overflowParameter);
    OpResult handleInternalNodeOverflow(OverflowParameter & overflowParameter, int blockPointer, VariableRecord *dataRecord);
    void handleOverflowInInternalNode(VariableRecord* keyAux, VariableRecord *dataRecord, OverflowParameter & overflowParameter, int newBlock);
    bool balanceLeafUnderflowRightWithinSameParent(LeafNode& leftLeaf, LeafNode& rightLeaf, TreeBlock* underflowBlock);
    bool balanceLeafUnderflowLeft(LeafNode& leftLeaf, LeafNode& rightLeaf, TreeBlock* underflowBlock);
    bool balanceLeafUnderflowRightWithinDifferentParents(LeafNode& node, LeafNode& brother, TreeBlock *underflowBlock, VariableRecord* record);
    OpResult handleLeafUnderflow(int nextNode, bool balanceRight, bool lastChild, bool leafAlreadyBalanced, LeafNode& node, VariableRecord* record);
    OpResult handleCrossParentBalance(VariableRecord *record, VariableRecord & aux);
public:
	InternalNode(TreeBlockFile* file, TreeBlock* b, RecordMethods* methods);
	virtual OpResult insert(VariableRecord* keyRecord, VariableRecord* dataRecord, OverflowParameter& overflowParameter);
	virtual OpResult update(char* key, VariableRecord* r);
	virtual OpResult remove(char* key, VariableRecord* record);
	virtual void print();
	virtual int getMaxSize();
	virtual int getMinimumSize();
	virtual ~InternalNode();
};

#endif /* INTERNALNODE_H_ */
