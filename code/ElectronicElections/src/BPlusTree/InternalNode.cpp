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

using namespace std;

InternalNode::InternalNode(TreeBlockFile* f, TreeBlock* b, RecordMethods* methods): Node(b, methods), file(f)
{
}

int InternalNode::getMaxSize()
{
	return (this->block->getSize() - IndexTreeBlock::RECORD_OFFSET);
}

OpResult InternalNode::handleLeafOverflow(VariableRecord* keyRecord, VariableRecord* dataRecord,
		VariableRecord* middleRecord, OverflowParameter& overflowParameter)
{
    TreeBlock *oldLeafBlock = this->file->getCurrentBlock();
    bool blockOne = true;
    if(this->recordMethods->compare(keyRecord->getBytes(),
    		middleRecord->getBytes(), middleRecord->getSize()) > 0)
    {
        blockOne = false;
    }
    int newBlock = this->file->getFreeBlockManager().getNextBlock();
    int newNextNode = this->file->getCurrentBlock()->getNextNode();
    this->file->getCurrentBlock()->setNextNode(newBlock);
    this->file->loadBlock(newBlock);
    this->file->pushBlock();
    this->file->getCurrentBlock()->setNextNode(newNextNode);
    this->file->getCurrentBlock()->setLevel(0);
    VariableRecord aux;
    stack<VariableRecord*> toRemove;
    oldLeafBlock->positionAtBegin();
    while(oldLeafBlock->getNextRecord(&aux) != NULL)
	{
		int result = this->recordMethods->compare
				(middleRecord->getBytes(), aux.getBytes(), aux.getSize());
		if (result <= 0)
		{
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
    if(blockOne)
    {
        oldLeafBlock->insertRecord(keyRecord, dataRecord);
    }
    else
    {
        this->file->getCurrentBlock()->insertRecord(keyRecord, dataRecord);
    }

    this->file->getCurrentBlock()->positionAtBegin();
    this->file->getCurrentBlock()->getNextRecord(&aux);
    this->file->saveBlock();
    this->file->popBlock();
    this->file->saveBlock();

    if (!this->block->canInsertRecord(middleRecord->getSize()))
    {
    	// Not enough space in the block!
    	overflowParameter.newBlock = newBlock;
		*middleRecord = aux;
    	return Overflow;
    }

    this->block->insertRecord(middleRecord, middleRecord);
    this->block->positionAtBegin();
    int index = 1;
    while(this->block->getNextRecord(&aux) != NULL && this->recordMethods->compare
			(middleRecord->getBytes(), aux.getBytes(), aux.getSize()) != 0)
    {
    	index++;
    }

    this->block->insertNodePointer(index, newBlock);

    return Updated;
}

OpResult InternalNode::insert(VariableRecord *keyRecord, VariableRecord *dataRecord,
		VariableRecord* middleRecord, OverflowParameter& overflowParameter)
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
		result = node.insert(keyRecord, dataRecord, middleRecord, overflowParameter);

		if (result == Updated)
		{
			this->file->saveBlock();
		}

		if (result == Overflow)
		{
			overflowParameter.overflowBlock = blockPointer;
		    result = this->handleLeafOverflow(keyRecord, dataRecord, middleRecord, overflowParameter);
		}
	}
	else
	{
		InternalNode node(this->file, this->file->getCurrentBlock(), this->recordMethods);
		result = node.insert(keyRecord, dataRecord, middleRecord, overflowParameter);
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
			cout << endl;
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
