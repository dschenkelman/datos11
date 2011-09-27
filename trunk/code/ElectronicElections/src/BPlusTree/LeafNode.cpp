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
	VariableRecord* variableRec = new VariableRecord();
	if(this->block->findRecord(key, &variableRec) != -1)
	{
		delete variableRec;
		return Duplicated;
	}

	delete variableRec;

	VariableRecord aux;

	if(!this->block->canInsertRecord(r->getSize()))
	{
		int bytes = 0;
		this->block->positionAtBegin();

		while (bytes < this->minimumSize && this->block->getNextRecord(&aux) != NULL)
		{
			bytes += aux.getSize();
			*r = aux;
		}

		return Overflow;
	}

	this->block->insertRecord(key, r);
	return Updated;
}

OpResult LeafNode::update(char *key, VariableRecord* r)
{
	VariableRecord* rec = new VariableRecord();
	int position = this->block->findRecord(key, &rec);
	if (position < 0)
	{
		delete rec;
		return NotFound;
	}

	short dif = r->getSize() - rec->getSize();

	delete rec;

	if (dif < 0 || this->block->getFreeSpace() > dif)
	{
		this->block->updateRecord(key, r);

		return Updated;
	}
	//else

	VariableRecord aux;
	int bytes = 0;
	this->block->positionAtBegin();

	while (bytes < this->minimumSize && this->block->getNextRecord(&aux) != NULL)
	{
		bytes += aux.getSize();
		*r = aux;
	}

	return Overflow;
}

OpResult LeafNode::remove(char *key)
{
	if (this->block->removeRecord(key))
	{
		if (this->block->getFreeSpace() > this->minimumSize)
		{
			return Underflow;
		}

		return Updated;
	}

	return NotFound;
}

void LeafNode::print()
{
	VariableRecord r;
	int i = 0;
	this->block->positionAtBegin();
	std::cout << "Leaf Node:";
	while(this->block->getNextRecord(&r) != NULL)
	{
		this->recordMethods->print(r.getBytes(), r.getSize());
		i++;
	}
}

LeafNode::~LeafNode()
{
}

