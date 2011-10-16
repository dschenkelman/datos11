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
	VariableRecord lastKey;
	TreeBlock* currentLeafBlock;
	Node* root;
	RecordMethods* methods;
	TreeBlockFile* file;
    void handleOverflowInLeafRoot(VariableRecord *dataRecord);
    void handleOverflowInInternalRoot(VariableRecord *dataRecord, OverflowParameter& overflowParameter);
    void changeBlock(int newBlock2, VariableRecord* middleRecord);
    void decreaseTreeHeight();
public:
	Tree(std::string fileName, int blockSize, RecordMethods* methods, bool createNew);
	OpResult insert(VariableRecord* keyRecord, VariableRecord* dataRecord);
	OpResult update(char* key, VariableRecord* r);
	void deleteKeptLeaf();
	bool get(char* key, VariableRecord* r);
	VariableRecord* getNext(VariableRecord* r);
	VariableRecord* returnFirst(VariableRecord* r);
	OpResult remove(char* key);
	void print();
	virtual ~Tree();
};

#endif /* TREE_H_ */
