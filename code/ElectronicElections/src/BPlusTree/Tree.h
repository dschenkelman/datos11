/*
 * Tree.h
 *
 *  Created on: Sep 30, 2011
 *      Author: damian
 */

#ifndef TREE_H_
#define TREE_H_

#include <string>
#include "../VariableBlocks/RecordMethods.h"
#include "TreeBlockFile.h"
#include "Node.h"
#include "OverflowParameter.h"

class Tree {
	Node* root;
	RecordMethods* methods;
	TreeBlockFile* file;
    void handleOverflowInLeafRoot(VariableRecord *keyRecord, VariableRecord& middleRecord, VariableRecord *dataRecord);
    void handleOverflowInInternalRoot(VariableRecord *keyRecord, VariableRecord& middleRecord,
    		VariableRecord *dataRecord, OverflowParameter& overflowParameter);
public:
	Tree(std::string fileName, int blockSize, RecordMethods* methods, bool createNew);
	OpResult insert(VariableRecord* keyRecord, VariableRecord* dataRecord);
	OpResult update(char* key, VariableRecord* r);
	OpResult remove(char* key);
	void print();
	virtual ~Tree();
};

#endif /* TREE_H_ */
