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
	this->minimumSize = this->getMinimumSize();
}

int LeafNode::getMinimumSize()
{
	return (this->block->getSize() - SequenceTreeBlock::RECORD_OFFSET) / 2;
}

OpResult LeafNode::insert(char* key, VariableRecord* r)
{
	this->block->insertRecord(key, r);
	return Updated;
}

OpResult LeafNode::remove(char *key)
{
	if (this->block->removeRecord(key) &&
			this->block->getFreeSpace() > this->minimumSize)
	{
		return Overflow;
	}

	// either not found and occupied size did not change, or the occupied size is OK
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

