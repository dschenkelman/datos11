/*
 * LeafNode.cpp
 *
 *  Created on: 20/09/2011
 *      Author: gonzalo
 */

#include "LeafNode.h"

LeafNode::LeafNode(TreeBlock* tb)
{
	this->treeBlock = tb;
	this->minimunNumberOfRecordsPerBlock = this->getMinimunNumberOfRecordsPerBlock();
	this->currentRecords = this->treeBlock->getOccupiedRecords();

	if(this->currentRecords < this->minimunNumberOfRecordsPerBlock)
	{
		// creo que esto no vale
	}
}

int LeafNode::getMinimunNumberOfRecordsPerBlock()
{
	return ceil(this->treeBlock->getRecordCount() / 2);
}

LeafNode::~LeafNode()
{
}

