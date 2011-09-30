/*
 * Tree.cpp
 *
 *  Created on: Sep 30, 2011
 *      Author: damian
 */

#include "Tree.h"
#include <fstream>
#include "LeafNode.h"
#include "InternalNode.h"

using namespace std;

Tree::Tree(string fileName, int blockSize, RecordMethods* methods, bool createNew = false)
{
	if (!createNew && !ifstream(fileName.c_str(), ifstream::in))
	{
		createNew = true;
	}

	this->file = new TreeBlockFile(fileName, blockSize, methods, createNew);
// load root
	this->file->loadBlock(0);
	if (createNew)
	{
		this->root = new LeafNode(this->file->getCurrentBlock(), methods);
		this->file->saveBlock();
	}
	else
	{
		if (this->file->isCurrentLeaf())
		{
			this->root = new LeafNode(this->file->getCurrentBlock(), methods);
		}
		else
		{
			this->root = new InternalNode(this->file, this->file->getCurrentBlock(), methods);
		}
	}
}

OpResult Tree::insert(VariableRecord *keyRecord, VariableRecord *dataRecord)
{
	OpResult result = this->root->insert(keyRecord, dataRecord);
	this->file->saveBlock();
	return result;
}

OpResult Tree::remove(char *key)
{
	return this->root->remove(key);
}

OpResult Tree::update(char *key, VariableRecord *r)
{
	return this->root->update(key, r);
}

void Tree::print()
{
	this->root->print();
}

Tree::~Tree()
{
	delete this->root;
	delete this->file;
}
