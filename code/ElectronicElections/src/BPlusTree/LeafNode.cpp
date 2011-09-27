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
	VariableRecord* variableRec = new VariableRecord;
	if(this->block->findRecord(key, &variableRec) != -1)
	{
		delete variableRec;
		return Duplicated;
	}

	if(!this->block->canInsertRecord(r->getSize()))
	{
		int bytes = 0;
		this->block->positionAtBegin();

		while (bytes < this->block->getSize()/2)
		{
			variableRec = this->block->getNextRecord(r);

			if(variableRec == NULL)
			{
				break; // no debería asignar el *r = *variableRec
			}

			bytes += variableRec->getSize();
		}

		*r = *variableRec;
		delete variableRec;
		return Overflow;
	}

	delete variableRec;

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

	if (dif < 0 || this->block->getFreeSpace() > dif)
	{
		this->block->updateRecord(key, r);
		delete rec;
		return Updated;
	}
	else
	{
		//|TODO: assign middle record to *r
		*r = *rec;
		delete rec;
		return Overflow;
	}

	delete rec;
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

