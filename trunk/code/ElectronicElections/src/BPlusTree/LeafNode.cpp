/*
 * LeafNode.cpp
 *
 *  Created on: 20/09/2011
 *      Author: gonzalo
 */

#include "LeafNode.h"
#include <iostream>

LeafNode::LeafNode(TreeBlock* tb, RecordMethods* methods)
: Node(tb, methods)
{
	this->minimumRecords = this->getMinimumRecords();
	this->recordCount = this->treeBlock->getOccupiedRecords();
}

int LeafNode::getMinimumRecords()
{
	return this->treeBlock->getRecordCount() / 2;
}

OpResult LeafNode::insert(char* key, Record* r)
{
	this->treeBlock->insertRecord(key, r->getBytes());
	return Updated;
}

void LeafNode::print()
{
	Record r(this->treeBlock-> getRecordSize());
	int i = 0;
	this->treeBlock->seekRecord(0);
	std::cout << "Leaf Node:";
	while(this->treeBlock->getNextRecord(&r) != NULL)
	{
		this->recordMethods->print(r.getBytes(), r.getSize());
		i++;
		if (i % this->treeBlock->getRecordCount() == 0)
		{
			std::cout << "..." << std::endl;
		}
	}
}

LeafNode::~LeafNode()
{
}
