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

class InternalNode: public Node
{
	TreeBlockFile* file;
public:
	InternalNode(TreeBlockFile* file, TreeBlock* b, RecordMethods* methods);
	virtual OpResult insert(VariableRecord* keyRecord, VariableRecord* dataRecord, VariableRecord* middleRecord);
	virtual OpResult update(char* key, VariableRecord* r);
	virtual OpResult remove(char* key);
	virtual void print();
	virtual ~InternalNode();
};

#endif /* INTERNALNODE_H_ */
