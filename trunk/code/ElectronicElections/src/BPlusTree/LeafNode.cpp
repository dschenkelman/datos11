/*
 * LeafNode.cpp
 *
 *  Created on: 20/09/2011
 *      Author: gonzalo
 */

#include "LeafNode.h"
#include <iostream>

LeafNode::LeafNode(SequenceTreeBlock* tb, RecordMethods* methods)
: Node(methods), block(tb)
{
	this->minimumSize = this->getMinimumSize();
}

int LeafNode::getMinimumSize()
{
	return this->block->getSize() / 2;
}

OpResult LeafNode::insert(char* key, VariableRecord* r)
{
	this->block->insertRecord(key, r);
	return Updated;
}

void LeafNode::print()
{
//	Record r(this->treeBlock-> getRecordSize());
//	int i = 0;
//	this->treeBlock->seekRecord(0);
//	std::cout << "Leaf Node:";
//	while(this->treeBlock->getNextRecord(&r) != NULL)
//	{
//		this->recordMethods->print(r.getBytes(), r.getSize());
//		i++;
//		if (i % this->treeBlock->getRecordCount() == 0)
//		{
//			std::cout << "..." << std::endl;
//		}
//	}
}

LeafNode::~LeafNode()
{
}

