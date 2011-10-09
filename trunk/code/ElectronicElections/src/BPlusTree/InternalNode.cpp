/*
 * InternalNode.cpp
 *
 *  Created on: Sep 30, 2011
 *      Author: damian
 */

#include "InternalNode.h"
#include "LeafNode.h"
#include <string.h>
#include <iostream>
#include <vector>
#include "IndexTreeBlock.h"
#include <math.h>

using namespace std;

InternalNode::InternalNode(TreeBlockFile* f, TreeBlock* b, RecordMethods* methods): Node(b, methods), file(f)
{
	this->maximumSize = this->calculateMaxSize();
}

int InternalNode::getMaxSize()
{
	return (this->block->getSize() - IndexTreeBlock::RECORD_OFFSET);
}

int InternalNode::calculateMaxSize()
{
	return floor(this->getMaxSize() * 0.9);
}

OpResult InternalNode::handleLeafOverflow(VariableRecord* keyRecord, VariableRecord* dataRecord,
		OverflowParameter& overflowParameter)
{
    TreeBlock *oldLeafBlock = this->file->getCurrentBlock();
    int newBlock = this->file->getFreeBlockManager().getNextBlock();
    int newNextNode = this->file->getCurrentBlock()->getNextNode();
    this->file->getCurrentBlock()->setNextNode(newBlock);
    this->file->loadBlock(newBlock);
    this->file->pushBlock();
    this->file->getCurrentBlock()->setNextNode(newNextNode);
    this->file->getCurrentBlock()->setLevel(0);
    bool middleInserted = false;
    VariableRecord aux;
    stack<VariableRecord*> toRemove;
    oldLeafBlock->positionAtBegin();
    while(oldLeafBlock->getNextRecord(&aux) != NULL)
	{
		int result = this->recordMethods->compare
				(dataRecord->getBytes(), aux.getBytes(), aux.getSize());
		if (result <= 0)
		{
			if (!middleInserted)
			{
				middleInserted = true;
				this->file->getCurrentBlock()->forceInsert(dataRecord);
			}
			this->file->getCurrentBlock()->forceInsert(&aux);

			VariableRecord* auxKeyRecord =
					this->recordMethods->getKeyRecord(aux.getBytes(), aux.getSize());
			toRemove.push(auxKeyRecord);
		}
	}

    while (!toRemove.empty())
    {
    	VariableRecord* auxKeyRecord = toRemove.top();
    	toRemove.pop();
    	oldLeafBlock->removeRecord(auxKeyRecord->getBytes());
    	delete auxKeyRecord;
    }

    this->file->getCurrentBlock()->positionAtBegin();
    this->file->getCurrentBlock()->getNextRecord(&aux);
    this->file->saveBlock();
    this->file->popBlock();
    this->file->saveBlock();

    VariableRecord* keyAux;

    keyAux = this->recordMethods->getKeyRecord(dataRecord->getBytes(), dataRecord->getSize());
    int freeSpace = this->block->getFreeSpace() - (keyAux->getSize() + Constants::RECORD_HEADER_SIZE + IndexTreeBlock::NODE_POINTER_SIZE);
    if (freeSpace < this->block->getSize() - this->maximumSize)
    {
    	// Not enough space in the block!
    	int index = 0;
    	int bytes = IndexTreeBlock::NODE_POINTER_SIZE;
		bool keyRecordConsidered = false;
		bool keyIsMiddle = false;
		this->block->positionAtBegin();

		while (this->block->getNextRecord(&aux) != NULL && bytes < (this->maximumSize / 2))
		{
			index++;
			if (!keyRecordConsidered &&
					this->recordMethods->compare(keyAux->getBytes(), aux.getBytes(), aux.getSize()) < 0)
			{
				keyRecordConsidered = true;
				bytes += dataRecord->getSize() + Constants::RECORD_HEADER_SIZE + IndexTreeBlock::NODE_POINTER_SIZE;
				if (bytes >= (this->maximumSize / 2))
				{
					keyIsMiddle = true;
					overflowParameter.newBlock = newBlock;
					aux = *keyAux;
					break;
				}
			}
			bytes += aux.getSize() + Constants::RECORD_HEADER_SIZE + IndexTreeBlock::NODE_POINTER_SIZE;
		}

		if (!keyRecordConsidered &&
									this->recordMethods->compare(keyRecord->getBytes(), aux.getBytes(), aux.getSize()) < 0)
		{
			keyRecordConsidered = true;
			keyIsMiddle = true;
			aux = *keyAux;
		}

		if (!keyIsMiddle)
		{
			this->block->removeRecord(aux.getBytes());
			this->block->insertRecord(keyRecord, dataRecord);
			overflowParameter.newBlock = this->block->getNodePointer(index);
			this->block->removeNodePointer(index);
			this->block->insertNodePointer(index, newBlock);
		}

		*dataRecord = aux;
		delete keyAux;
    	return Overflow;
    }

    this->block->insertRecord(keyAux, keyAux);
    this->block->positionAtBegin();
    int index = 1;
    while(this->block->getNextRecord(&aux) != NULL && this->recordMethods->compare
			(keyAux->getBytes(), aux.getBytes(), aux.getSize()) != 0)
    {
    	index++;
    }

    this->block->insertNodePointer(index, newBlock);

    delete keyAux;

    return Updated;
}

OpResult InternalNode::insert(VariableRecord *keyRecord, VariableRecord *dataRecord, OverflowParameter& overflowParameter)
{
	VariableRecord aux;
	this->block->positionAtBegin();
	int index = 0;
	while(this->block->getNextRecord(&aux) != NULL)
	{
		if(this->recordMethods->compare
				(keyRecord->getBytes(), aux.getBytes(), aux.getSize()) < 0)
		{
			break;
		}

		index++;
	}

	int blockPointer = this->block->getNodePointer(index);
	this->file->loadBlock(blockPointer);
	this->file->pushBlock();

	OpResult result;

	if (this->file->isCurrentLeaf())
	{
		LeafNode node(this->file->getCurrentBlock(), this->recordMethods);
		result = node.insert(keyRecord, dataRecord, overflowParameter);

		if (result == Updated)
		{
			this->file->saveBlock();
			result = Unchanged;
		}

		if (result == Overflow)
		{
			overflowParameter.overflowBlock = blockPointer;
		    result = this->handleLeafOverflow(keyRecord, dataRecord, overflowParameter);
		}
	}
	else
	{
		InternalNode node(this->file, this->file->getCurrentBlock(), this->recordMethods);
		result = node.insert(keyRecord, dataRecord, overflowParameter);

		if (result == Updated)
		{
			this->file->saveBlock();
			result = Unchanged;
		}

		if (result == Overflow)
		{
			overflowParameter.overflowBlock = blockPointer;
			TreeBlock* overflowBlock = this->file->getCurrentBlock();
			int newBlock = this->file->getFreeBlockManager().getNextBlock();
			this->file->loadBlock(newBlock);
			this->file->pushBlock();
			this->file->swapBlockKind();
			this->file->getCurrentBlock()->setLevel(overflowBlock->getLevel());

			this->block->insertRecord(dataRecord, dataRecord);
			VariableRecord aux;
			int index = 0;
			while (this->block->getNextRecord(&aux) &&
					this->recordMethods->compare(aux.getBytes(), dataRecord->getBytes(), dataRecord->getSize()) != 0)
			{
				index++;
			}

			this->block->insertNodePointer(index, newBlock);

			result = Updated;
		}
	}

	// restore block
	this->file->popBlock();
	this->file->saveBlock();

	return result;
}

void InternalNode::print()
{
	VariableRecord aux;
	this->block->positionAtBegin();
	int i = 0;
	while(this->block->getNextRecord(&aux) != NULL)
	{
		cout << this->block->getNodePointer(i);
		i++;
		cout << "(";
		this->recordMethods->printKey(aux.getBytes(), aux.getSize());
		cout << ")";
	}

	cout << this->block->getNodePointer(i) << endl;

	for (int j = 0; j <= i; j++)
	{
		int blockPointer = this->block->getNodePointer(j);
		this->file->loadBlock(blockPointer);
		this->file->pushBlock();
		if (this->file->isCurrentLeaf())
		{
			LeafNode node(this->file->getCurrentBlock(), this->recordMethods);
			node.print();
			cout << endl;
		}
		else
		{
			InternalNode node(this->file, this->file->getCurrentBlock(), this->recordMethods);
			node.print();
		}

		this->file->popBlock();
	}
}

OpResult InternalNode::remove(char *key)
{
	VariableRecord aux;
	OpResult result;
	this->block->positionAtBegin();
	int index;
	while(this->block->getNextRecord(&aux) != NULL)
	{
		if(this->recordMethods->compare
				(key, aux.getBytes(), aux.getSize()) < 0)
		{
			break;
		}

		index++;
	}


	int blockPointer = this->block->getNodePointer(index);
	this->file->loadBlock(blockPointer);
	this->file->pushBlock();

	if (this->file->isCurrentLeaf())
	{
		LeafNode node(this->file->getCurrentBlock(), this->recordMethods);
		result = node.remove(key);
	}
	else
	{
		InternalNode node(this->file, this->file->getCurrentBlock(), this->recordMethods);
		result = node.remove(key);
	}

	if (result == Underflow)
	{
		if (this->file->isCurrentLeaf())
		{
			// Leaf in Underflow
			// Preguntar si sobran records en las hojas vecinas
		}
		else
		{
			// InternalNode in Underflow
		}
	}

	// save block
	this->file->saveBlock();

	// restore block
	this->file->popBlock();

	return result;
}

OpResult InternalNode::update(char *key, VariableRecord *r)
{
	return Updated;
}

InternalNode::~InternalNode()
{
}
