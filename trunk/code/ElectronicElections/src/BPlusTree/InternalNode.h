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
	int minimumSize;
	int maximumSize;
	TreeBlockFile* file;
	int calculateMaxSize();
	int calculateMinimumSize();
    OpResult handleLeafOverflow(VariableRecord* dataRecord,
    		OverflowParameter& overflowParameter);
    OpResult handleInternalNodeOverflow(OverflowParameter& overflowParameter,
    		int blockPointer, VariableRecord *dataRecord);
    void handleOverflowInInternalNode(VariableRecord* keyAux, VariableRecord *dataRecord, OverflowParameter & overflowParameter, int newBlock);
    bool balanceLeafUnderflowRight(LeafNode& leftLeaf, LeafNode& rightLeaf, TreeBlock* underflowBlock);
    bool balanceLeafUnderflowLeft(LeafNode& leftLeaf, LeafNode& rightLeaf, TreeBlock* underflowBlock, char* removedKey);

    bool balanceLeafUnderflowRightWithinDifferentParents(LeafNode& node, LeafNode& brother, TreeBlock *underflowBlock, VariableRecord* record);
    OpResult handleLeafUnderflow(int nextNode, bool lastChild,
    		bool *leafAlreadyBalanced, LeafNode& node, char* key, int index);

    OpResult handleCrossParentBalance(VariableRecord *record, VariableRecord & aux);
    void mergeLeafNodes(int index, TreeBlock *brotherBlock, TreeBlock *underflowBlock, char* key, bool lastChild);
    bool balanceInternalNodeToTheLeft(TreeBlock *underflowBlock, TreeBlock *balancingBlock, VariableRecord & aux);
    bool balanceInternalNodeToTheRight(TreeBlock *balancingBlock, TreeBlock *underflowBlock);
    void mergeNodeToTheLeft(VariableRecord & nextKeyInFather, int index, TreeBlock *underflowBlock, TreeBlock *balancingBlock);
    void mergeLastInternalChildNode(VariableRecord & lastRecord, int index, TreeBlock *balancingBlock, TreeBlock *underflowBlock);
public:
	InternalNode(TreeBlockFile* file, TreeBlock* b, RecordMethods* methods);
	virtual OpResult insert(VariableRecord* keyRecord, VariableRecord* dataRecord, OverflowParameter& overflowParameter);
	virtual OpResult update(char* key, VariableRecord* r, OverflowParameter& overflowParameter);
	virtual OpResult remove(char* key);
	bool get(char* key, VariableRecord* record);
	virtual VariableRecord* returnFirst(VariableRecord* r);
	virtual void print();
	virtual bool isUnderflow();
	virtual int getMaxSize();
	virtual int getMinimumSize();
	virtual ~InternalNode();
};

#endif /* INTERNALNODE_H_ */
