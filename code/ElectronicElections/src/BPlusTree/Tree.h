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
#include "TreeBlockFile.h"this->getCurrentBlock()->clear();
#include "Node.h"

class Tree {
	Node* root;
	RecordMethods* methods;
	TreeBlockFile* file;
public:
	Tree(std::string fileName, int blockSize, RecordMethods* methods, bool createNew);
	OpResult insert(VariableRecord* keyRecord, VariableRecord* dataRecord);
	OpResult update(char* key, VariableRecord* r);
	OpResult remove(char* key);
	void print();
	virtual ~Tree();
};

#endif /* TREE_H_ */
