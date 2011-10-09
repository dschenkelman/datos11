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

class InternalNode: public Node
{
	TreeBlockFile* file;
	int maximumSize;
	int calculateMaxSize();
    OpResult handleLeafOverflow(VariableRecord* keyRecord, VariableRecord* dataRecord, OverflowParameter& overflowParameter);
public:
	InternalNode(TreeBlockFile* file, TreeBlock* b, RecordMethods* methods);
	virtual OpResult insert(VariableRecord* keyRecord, VariableRecord* dataRecord, OverflowParameter& overflowParameter);
	virtual OpResult update(char* key, VariableRecord* r);
	virtual OpResult remove(char* key);
	virtual void print();
	virtual int getMaxSize();
	virtual ~InternalNode();
};

#endif /* INTERNALNODE_H_ */
