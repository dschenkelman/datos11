/*
 * InternalNode.cpp
 *
 *  Created on: Sep 30, 2011
 *      Author: damian
 */

#include "InternalNode.h"
#include "LeafNode.h"
#include <string.h>
#include <cstdlib>
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

void InternalNode::handleOverflowInInternalNode(VariableRecord& aux, VariableRecord* keyAux,
		VariableRecord* dataRecord, OverflowParameter& overflowParameter, int newBlock)
{
    int index = 0;
    int bytes = IndexTreeBlock::NODE_POINTER_SIZE;
    bool keyRecordConsidered = false;
    bool keyIsMiddle = false;
    this->block->positionAtBegin();
    while (this->block->getNextRecord(&aux) != NULL && ++index &&  bytes < (this->maximumSize / 2))
	{
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

    if(!keyRecordConsidered &&
    		this->recordMethods->compare(keyAux->getBytes(), aux.getBytes(), aux.getSize()) < 0)
    {
        keyRecordConsidered = true;
        keyIsMiddle = true;
        aux = *keyAux;
    }

    if(!keyIsMiddle)
    {
        this->block->removeRecord(aux.getBytes());
        this->block->insertRecord(keyAux, keyAux);
        overflowParameter.newBlock = this->block->getNodePointer(index);
        this->block->removeNodePointer(index);
        this->block->insertNodePointer(index, newBlock);
    }
    *dataRecord = aux;
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

    this->file->saveBlock();
    this->file->popBlock();
    this->file->saveBlock();

    VariableRecord* keyAux;

    keyAux = this->recordMethods->getKeyRecord(dataRecord->getBytes(), dataRecord->getSize());
    int freeSpace = this->block->getFreeSpace() - (keyAux->getSize() + Constants::RECORD_HEADER_SIZE + IndexTreeBlock::NODE_POINTER_SIZE);
    if (freeSpace < this->block->getSize() - this->maximumSize)
    {
        this->handleOverflowInInternalNode(aux, keyAux, dataRecord, overflowParameter, newBlock);
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
			int index = 1;
			this->block->positionAtBegin();
			while (this->block->getNextRecord(&aux) &&
					this->recordMethods->compare(aux.getBytes(), dataRecord->getBytes(), dataRecord->getSize()) != 0)
			{
				index++;
			}

			this->block->insertNodePointer(index, newBlock);

			overflowBlock->positionAtBegin();
			index = 1;
			int index2 = 0;
			bool middlePointerAdded = false;
			stack<VariableRecord*> recordsToDelete;
			stack<int> nodesToDelete;
			VariableRecord* aux2;
			while(overflowBlock->getNextRecord(&aux))
			{
				if (this->recordMethods->compare(aux.getBytes(), dataRecord->getBytes(), dataRecord->getSize()) > 0)
				{
					if (!middlePointerAdded)
					{
						middlePointerAdded = true;
						this->file->getCurrentBlock()->insertNodePointer(index2, overflowParameter.newBlock);
						index2++;
					}

					aux2 = new VariableRecord;
					*aux2 = aux;
					int nodePointer = -1;
					nodePointer = overflowBlock->getNodePointer(index);
					this->file->getCurrentBlock()->insertRecord(aux2, aux2);
					recordsToDelete.push(aux2);
					nodesToDelete.push(index);
					this->file->getCurrentBlock()->insertNodePointer(index2, nodePointer);
					index2++;

				}
				index++;
			}

			int nodePointer = overflowBlock->getNodePointer(index);
			this->file->getCurrentBlock()->insertNodePointer(index2, nodePointer);
			nodesToDelete.push(index);

			while(!recordsToDelete.empty())
			{
				aux2 = recordsToDelete.top();
				recordsToDelete.pop();
				overflowBlock->removeRecord(aux2->getBytes());
				index = nodesToDelete.top();
				nodesToDelete.pop();
				overflowBlock->removeNodePointer(index);
			}

			index = nodesToDelete.top();
			nodesToDelete.pop();
			overflowBlock->removeNodePointer(index);

			this->file->saveBlock();
			this->file->popBlock();
			this->file->saveBlock();
			this->block->setLevel(this->block->getLevel() + 1);
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
	bool leafAlreadyBalanced = false;
	this->block->positionAtBegin();

	// Look for the Leaf
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
	// ======================= //


	if (this->file->isCurrentLeaf())
	{
		LeafNode node(this->file->getCurrentBlock(), this->recordMethods);
		result = node.remove(key);
	}
	else
	{
		exit(1);
		/*	PENDING
		InternalNode node(this->file, this->file->getCurrentBlock(), this->recordMethods);
		result = node.remove(key);*/
	}

	if (result == Underflow)
	{

		if (this->file->isCurrentLeaf())
		{
			// Leaf in Underflow
			LeafNode underflowLeaf(this->file->getCurrentBlock(), this->recordMethods);

			// Look for right brother
			int nextNodePointer = this->block->getNodePointer(index+1);
			if (nextNodePointer != -1)
			{
				//Have a right brother
				this->file->loadBlock(nextNodePointer);
				this->file->pushBlock();
				LeafNode rightBrother(this->file->getCurrentBlock(), this->recordMethods);
				leafAlreadyBalanced = this->handleLeafUnderflow(&underflowLeaf,&rightBrother);

			}
			if (!leafAlreadyBalanced)
			{
				//There is not enough records in right brother (or it does not exist)
				// Look for left brother
				int nextNodePointer = this->block->getNodePointer(index-1);
				if (nextNodePointer != -1)
				{
					//Have a left brother
					this->file->loadBlock(nextNodePointer);
					this->file->pushBlock();
					LeafNode leftBrother(this->file->getCurrentBlock(), this->recordMethods);
					leafAlreadyBalanced  = this->handleLeafUnderflow(&leftBrother,&underflowLeaf);
				}
			}
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

bool InternalNode::handleLeafUnderflow(LeafNode* underflowLeaf, LeafNode* brotherLeaf)
{
	int totalCapacity = underflowLeaf->getOccupiedSize() + brotherLeaf->getOccupiedSize();
	OverflowParameter parameter;
	if ( ((totalCapacity / 2) * 0.9) < underflowLeaf->getMinimumSize() )	// 90% * 50% = Underflow limit at 45%
	{
		// There is not enough records to balance the Leafs
		return false;
	}

	while (!underflowLeaf->getOccupiedSize() < brotherLeaf->getOccupiedSize())
	{
		VariableRecord* dataRecord = brotherLeaf->popFirst();
		VariableRecord* keyRecord = this->recordMethods->getKeyRecord(dataRecord->getBytes(),dataRecord->getSize());
		underflowLeaf->insert(keyRecord, dataRecord, parameter);
	}


	return true;
}

int InternalNode::getMinimumSize()
{
	//TODO
	return 0;
}

OpResult InternalNode::update(char *key, VariableRecord *r)
{
	return Updated;
}

InternalNode::~InternalNode()
{
}
