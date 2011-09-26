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

OpResult LeafNode::update(char *key, VariableRecord* r)
{
	VariableRecord* rec = new VariableRecord();
	int position = this->block->findRecord(key, &rec);
	if (position < 0)
	{
		return NotFound;
	}

	short dif = r->getSize() - rec->getSize();

	if (dif < 0 || this->block->getFreeSpace() > dif)
	{
		this->block->updateRecord(key, r);
		return Updated;
	}
	else
	{
		// should assign middle record to *r
		return Overflow;
	}

	delete red;
}

OpResult LeafNode::remove(char *key)
{
	if (this->block->removeRecord(key))
	{
		if (this->block->getFreeSpace() > this->minimumSize)
		{
			return Underflow;
		}

		return NotFound;
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

