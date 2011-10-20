/*
 * LeafNode.cpp
 *
 *  Created on: 20/09/2011
 *      Author: gonzalo
 */

#include "LeafNode.h"
#include <iostream>
#include <math.h>

LeafNode::LeafNode(TreeBlock* tb, RecordMethods* methods)
: Node(tb, methods)
{
	this->minimumSize = this->calculateMinimumSize();
	this->maximumSize = this->calculateMaximumSize();
	this->block->updateInformation();
}

int LeafNode::calculateMinimumSize()
{
	return (floor(this->block->getSize() - SequenceTreeBlock::RECORD_OFFSET) * 0.45);
}

int LeafNode::calculateMaximumSize()
{
	return floor((this->block->getSize() - SequenceTreeBlock::RECORD_OFFSET) * 0.9);
}

int LeafNode::getMinimumSize()
{
	return this->calculateMinimumSize();
}

VariableRecord* LeafNode::popFirst()
{
	return this->block->popFirst();
}

VariableRecord *LeafNode::popLast()
{
	return this->block->popLast();
}

OpResult LeafNode::insert(VariableRecord* keyRecord, VariableRecord* dataRecord, OverflowParameter& overflowParameter)
{
	//Check uniqueness
	VariableRecord* variableRec = NULL;
	if(this->block->findRecord(keyRecord->getBytes(), &variableRec) != -1)
	{
		if (variableRec != NULL)
		{
			delete variableRec;
		}

		return Duplicated;
	}

	if (variableRec != NULL)
	{
		delete variableRec;
	}

	VariableRecord aux;

	int freeSpace = this->block->getFreeSpace() - (dataRecord->getSize() + Constants::RECORD_HEADER_SIZE);
	if(freeSpace >= this->block->getSize() - this->maximumSize)
	{
		this->block->insertRecord(keyRecord, dataRecord);
		return Updated;
	}

	// Not enough space in the block!
	int bytes = 0;
	bool dataRecordConsidered = false;
	bool dataIsMiddle = false;
	this->block->positionAtBegin();

	while (bytes < (this->maximumSize / 2) && this->block->getNextRecord(&aux) != NULL)
	{
		if (!dataRecordConsidered &&
				this->recordMethods->compare(keyRecord->getBytes(), aux.getBytes(), aux.getSize()) < 0)
		{
			dataRecordConsidered = true;
			bytes += dataRecord->getSize() + Constants::RECORD_HEADER_SIZE;
			if (bytes >= (this->maximumSize / 2))
			{
				dataIsMiddle = true;
				aux = *dataRecord;
				break;
			}
		}
		bytes += aux.getSize() + Constants::RECORD_HEADER_SIZE;
	}

	if (!dataRecordConsidered &&
					this->recordMethods->compare(keyRecord->getBytes(), aux.getBytes(), aux.getSize()) < 0)
	{
		dataRecordConsidered = true;
		dataIsMiddle = true;
		aux = *dataRecord;
	}

	if (!dataIsMiddle)
	{
		VariableRecord* keyAux = this->recordMethods->getKeyRecord(aux.getBytes(), aux.getSize());
		this->block->removeRecord(keyAux->getBytes());
		this->block->insertRecord(keyRecord, dataRecord);
		delete keyAux;
	}

	*dataRecord = aux;

	return Overflow;
}

OpResult LeafNode::update(char *key, VariableRecord* r, OverflowParameter& overflowParameter)
{
	VariableRecord* rec = NULL;
	int position = this->block->findRecord(key, &rec);
	if (position < 0)
	{
		if (rec != NULL)
		{
			delete rec;
		}

		return NotFound;
	}

	short dif = r->getSize() - rec->getSize();

	if (rec != NULL)
	{
		delete rec;
	}

	if (dif < 0 || this->maximumSize >= dif + this->block->getOccupiedSize())
	{
		this->block->updateRecord(key, r);
		return Updated;
	}

	int bytes = 0;
	this->block->positionAtBegin();
	VariableRecord aux;

	while (bytes < (this->maximumSize / 2) && this->block->getNextRecord(&aux) != NULL)
	{
		if (this->recordMethods->compare(key, aux.getBytes(), aux.getSize()) == 0)
		{
			aux = *r;
			if (r->getSize() >= this->maximumSize / 2)
			{
				break;
			}
			bytes += r->getSize() + Constants::RECORD_HEADER_SIZE;
		}
		else
		{
			if (aux.getSize() >= this->maximumSize / 2)
			{
				break;
			}
			bytes += aux.getSize() + Constants::RECORD_HEADER_SIZE;
		}
	}

	if (this->recordMethods->compare(key, aux.getBytes(), aux.getSize()) != 0)
	{
		VariableRecord* keyAux = this->recordMethods->getKeyRecord(aux.getBytes(), aux.getSize());
		this->block->removeRecord(keyAux->getBytes());
		this->block->updateRecord(key, r);
		delete keyAux;
	}
	else
	{
		this->block->removeRecord(key);
	}

	*r = aux;
	return Overflow;
}

OpResult LeafNode::remove(char *key)
{
	if (this->block->removeRecord(key))
	{
		int occupiedSpace = this->block->getOccupiedSize() - SequenceTreeBlock::RECORD_OFFSET;
		if (occupiedSpace < this->minimumSize)
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

int LeafNode::getMaxSize()
{
	return (this->block->getSize() - SequenceTreeBlock::RECORD_OFFSET);
}

int LeafNode::getNextNode()
{
	return this->block->getNextNode();
}

bool LeafNode::isUnderflow()
{
	// TODO if required
	return false;
}

GetResult LeafNode::get(char* key, VariableRecord* record)
{
	VariableRecord* rec = NULL;
	GetResult result = this->block->findEqualOrGreaterRecord(key, &rec);

	if (result == Equal || result == Greater)
	{
		record->setBytes(rec->getBytes(),rec->getSize());
		delete rec;
	}

	return result;
}

VariableRecord* LeafNode::returnFirst(VariableRecord* r)
{
	this->block->positionAtBegin();
	this->block->getNextRecord(r);
	return r;
}

LeafNode::~LeafNode()
{
}
