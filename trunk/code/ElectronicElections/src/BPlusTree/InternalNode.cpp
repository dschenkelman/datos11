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
	this->minimumSize = this->calculateMinimumSize();
}

int InternalNode::getMaxSize()
{
	return (this->block->getSize() - IndexTreeBlock::RECORD_OFFSET);
}

int InternalNode::calculateMaxSize()
{
	return floor(this->getMaxSize() * 0.9);
}

void InternalNode::handleOverflowInInternalNode(VariableRecord* keyAux,
		VariableRecord* dataRecord, OverflowParameter& overflowParameter, int newBlock)
{
	VariableRecord aux;
	int index = 0;
    int bytes = IndexTreeBlock::NODE_POINTER_SIZE;
    bool keyRecordConsidered = false;
    bool keyIsMiddle = false;
    this->block->positionAtBegin();
    while (bytes < (this->maximumSize / 2) && this->block->getNextRecord(&aux) != NULL && ++index)
	{
		if (!keyRecordConsidered &&
				this->recordMethods->compare(keyAux->getBytes(), aux.getBytes(), aux.getSize()) < 0)
		{
			keyRecordConsidered = true;
			bytes += keyAux->getSize() + Constants::RECORD_HEADER_SIZE + IndexTreeBlock::NODE_POINTER_SIZE;
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
        index = 0;
        this->block->positionAtBegin();
        VariableRecord aux2;
        while(this->block->getNextRecord(&aux2) != NULL)
        {
        	index++;
        	if (this->recordMethods->compare(keyAux->getBytes(), aux2.getBytes(), aux2.getSize()) == 0)
        	{
        		break;
        	}
        }
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
    int freeSpace = this->block->getFreeSpace() - (keyAux->getSize() +
    		Constants::RECORD_HEADER_SIZE + IndexTreeBlock::NODE_POINTER_SIZE);
    if (freeSpace < this->block->getSize() - IndexTreeBlock::RECORD_OFFSET - this->maximumSize)
    {
        this->handleOverflowInInternalNode(keyAux, dataRecord, overflowParameter, newBlock);
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

OpResult InternalNode::handleInternalNodeOverflow(OverflowParameter& overflowParameter, int blockPointer, VariableRecord *dataRecord)
{
	VariableRecord aux;
    overflowParameter.overflowBlock = blockPointer;
    TreeBlock *overflowBlock = this->file->getCurrentBlock();
    int newBlock = this->file->getFreeBlockManager().getNextBlock();
    this->file->loadBlock(newBlock);
    this->file->pushBlock();
    this->file->swapBlockKind();
    this->file->getCurrentBlock()->setLevel(overflowBlock->getLevel());
    overflowBlock->positionAtBegin();
    int index = 0;
    int index2 = 0;
    bool middlePointerAdded = false;
    stack<VariableRecord*> recordsToDelete;
    stack<int> nodesToDelete;
    VariableRecord *aux2;
    int nodePointer = -1;
    while(overflowBlock->getNextRecord(&aux) != NULL)
    {
        index++;
        if(this->recordMethods->compare(aux.getBytes(), dataRecord->getBytes(), dataRecord->getSize()) > 0)
        {
            if(!middlePointerAdded)
            {
                middlePointerAdded = true;
                this->file->getCurrentBlock()->insertNodePointer(index2, overflowParameter.newBlock);
                index2++;
            }
            aux2 = new VariableRecord;
            *aux2 = aux;
            nodePointer = -1;
            nodePointer = overflowBlock->getNodePointer(index);
            this->file->getCurrentBlock()->insertRecord(aux2, aux2);
            recordsToDelete.push(aux2);
            nodesToDelete.push(index);
            this->file->getCurrentBlock()->insertNodePointer(index2, nodePointer);
            index2++;
        }
    }

    nodePointer = overflowBlock->getNodePointer(index);
    this->file->getCurrentBlock()->insertNodePointer(index2, nodePointer);
    nodesToDelete.push(index);
    while(!recordsToDelete.empty())
    {
        aux2 = recordsToDelete.top();
        recordsToDelete.pop();
        overflowBlock->removeRecord(aux2->getBytes());
        delete aux2;
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

    VariableRecord* keyAux;
    keyAux = this->recordMethods->getKeyRecord(dataRecord->getBytes(), dataRecord->getSize());
	int freeSpace = this->block->getFreeSpace() - (keyAux->getSize()
			+ Constants::RECORD_HEADER_SIZE + IndexTreeBlock::NODE_POINTER_SIZE);
	if (freeSpace < this->block->getSize() - this->maximumSize - IndexTreeBlock::RECORD_OFFSET)
	{
		this->handleOverflowInInternalNode(keyAux, dataRecord, overflowParameter, newBlock);
		delete keyAux;
		return Overflow;
	}

    this->block->insertRecord(dataRecord, dataRecord);
	index = 1;
	this->block->positionAtBegin();
	while(this->block->getNextRecord(&aux) && this->recordMethods->compare(aux.getBytes(), dataRecord->getBytes(), dataRecord->getSize()) != 0)
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
		    result = this->handleInternalNodeOverflow(overflowParameter, blockPointer, dataRecord);
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

OpResult InternalNode::handleLeafUnderflow(int nextNode, bool lastChild,
		bool leafAlreadyBalanced, LeafNode& node, char* key, int index)
{
    TreeBlock *underflowBlock = this->file->getCurrentBlock();
    this->file->loadBlock(nextNode);
    this->file->pushBlock();
    LeafNode brother(this->file->getCurrentBlock(), this->recordMethods);
    if(!lastChild)
    {
		leafAlreadyBalanced = this->balanceLeafUnderflowRight(node, brother, underflowBlock);
    }
    else
    {
        leafAlreadyBalanced = this->balanceLeafUnderflowLeft(brother, node, underflowBlock, key);
    }

    if(!leafAlreadyBalanced)
    {
    	if (!lastChild)
		{
    		this->mergeLeafNodes(index, this->file->getCurrentBlock(), underflowBlock, key, lastChild);
		}
    	else
    	{
    		// underflow node is the last (right-sided)
    		this->mergeLeafNodes(index-1, underflowBlock, this->file->getCurrentBlock(), key, lastChild);
    	}

    	if ((this->block->getOccupiedSize() - IndexTreeBlock::RECORD_OFFSET) < this->minimumSize)
    	{
    		return Underflow;
    	}
    	return Updated;
    }
    else
    {
        return Unchanged;
    }
}

OpResult InternalNode::handleCrossParentBalance(VariableRecord *record, VariableRecord& aux)
{
    VariableRecord aux2;
    VariableRecord *keyRecord = this->recordMethods->getKeyRecord(record->getBytes(), record->getSize());
    this->block->positionAtBegin();
    while (this->block->getNextRecord(&aux) != NULL)
			{
				if (this->recordMethods->compare
						(keyRecord->getBytes(), aux.getBytes(), aux.getSize()) < 0)
				{
					break;
				}

				aux2 = aux;
			}
    VariableRecord *keyAux = this->recordMethods->getKeyRecord(aux2.getBytes(), aux2.getSize());
    this->block->removeRecord(keyAux->getBytes());
    this->block->insertRecord(keyRecord, record);
    delete keyAux;
    delete keyRecord;
    return Updated;
}

void InternalNode::mergeLeafNodes(int index, TreeBlock *brotherBlock, TreeBlock *underflowBlock, char* removedKey, bool lastChild)
{
    // InternalNode Stuff
	VariableRecord aux;
    VariableRecord *keyAux;
    this->block->removeNodePointer(index + 1);
    brotherBlock->positionAtBegin();
    brotherBlock->getNextRecord(&aux);
    keyAux = this->recordMethods->getKeyRecord(aux.getBytes(), aux.getSize());
    if (lastChild)
    {
    	if (this->recordMethods->compare(removedKey, aux.getBytes(), aux.getSize()) < 0)
		{
			this->block->removeRecord(removedKey);
		}
		else
		{
			this->block->removeRecord(keyAux->getBytes());
		}
    }
    else
    {
    	this->block->removeRecord(keyAux->getBytes());
    }

    delete keyAux;
    while(!brotherBlock->isEmpty())
    {
        VariableRecord *record = brotherBlock->popLast();
        VariableRecord *keyAux;
        keyAux = this->recordMethods->getKeyRecord(record->getBytes(), record->getSize());
        underflowBlock->insertRecord(keyAux, record);
        delete record;
        delete keyAux;
    }
    underflowBlock->setNextNode(brotherBlock->getNextNode());
}

void InternalNode::balanceInternalNodeToTheLeft(TreeBlock *underflowBlock, TreeBlock *balancingBlock, VariableRecord & aux)
{
	int underflowRecordCount = 0;
	VariableRecord aux2;
	underflowBlock->positionAtBegin();
	while (underflowBlock->getNextRecord(&aux2)){ underflowRecordCount++; }
    while(underflowBlock->getOccupiedSize() < balancingBlock->getOccupiedSize()){
        underflowBlock->forceInsert(&aux);
        underflowRecordCount++;
        this->block->removeRecord(aux.getBytes());
        balancingBlock->positionAtBegin();
        balancingBlock->getNextRecord(&aux);
        int nodePointer = balancingBlock->getNodePointer(0);
        balancingBlock->removeRecord(aux.getBytes());
        balancingBlock->removeNodePointer(0);
        this->block->insertRecord(&aux, &aux);
        underflowBlock->insertNodePointer(underflowRecordCount, nodePointer);
    }
}

void InternalNode::balanceInternalNodeToTheRight(TreeBlock *balancingBlock, TreeBlock *underflowBlock)
{
	VariableRecord aux;
    int balanceRecordCount = 0;
    balancingBlock->positionAtBegin();
    while(balancingBlock->getNextRecord(&aux)){
        balanceRecordCount++;
    }
    while(balancingBlock->getOccupiedSize() > underflowBlock->getOccupiedSize())
    {
        VariableRecord *lastParentRecord = this->block->popLast();
        int futureUnderflowSize = lastParentRecord->getSize() + underflowBlock->getOccupiedSize() + Constants::RECORD_HEADER_SIZE;
        if(futureUnderflowSize > balancingBlock->getOccupiedSize()){
            this->block->forceInsert(lastParentRecord);
            delete lastParentRecord;
            break;
        }
        underflowBlock->insertRecord(lastParentRecord, lastParentRecord);
        int nodePointer = balancingBlock->getNodePointer(balanceRecordCount);
        VariableRecord *lastBalancingRecord = balancingBlock->popLast();
        balanceRecordCount--;
        underflowBlock->insertNodePointer(0, nodePointer);
        this->block->insertRecord(lastBalancingRecord, lastBalancingRecord);
        delete lastParentRecord;
        delete lastBalancingRecord;
    }
}

OpResult InternalNode::remove(char *key)
{
	VariableRecord aux;
	OpResult result;
	bool leafAlreadyBalanced = false;
	this->block->positionAtBegin();

	// Look for the Leaf
	int index = 0;
	bool lastChild = true;
	while(this->block->getNextRecord(&aux) != NULL)
	{
		if(this->recordMethods->compare
				(key, aux.getBytes(), aux.getSize()) < 0)
		{
			lastChild = false;
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


		if (result == Underflow)
		{
			// Look for brother to balance with
			int nextNode = node.getNextNode();
			nextNode = !lastChild ? nextNode : this->block->getNodePointer(index-1);

			if (nextNode != 0)
			{
				result = this->handleLeafUnderflow(nextNode, lastChild,
				leafAlreadyBalanced, node, key, index);
				this->file->saveBlock();
				this->file->popBlock();
			}
		}
	}
	else
	{
		InternalNode node(this->file, this->file->getCurrentBlock(), this->recordMethods);
		result = node.remove(key);

		if (result == Underflow)
		{
			int balancingNode = lastChild ? this->block->getNodePointer(index - 1) : this->block->getNodePointer(index + 1);
			TreeBlock* underflowBlock = this->file->getCurrentBlock();
			this->file->loadBlock(balancingNode);
			this->file->pushBlock();
			TreeBlock* balancingBlock = this->file->getCurrentBlock();
		    if (lastChild)
		    {
    		    this->balanceInternalNodeToTheRight(balancingBlock, underflowBlock);
    		}
		    else
		    {
		    	this->balanceInternalNodeToTheLeft(underflowBlock, balancingBlock, aux);
		    }
		    this->file->saveBlock();
			this->file->popBlock();

			result = Updated;
		}
	}

	// save block
	this->file->saveBlock();

	// restore block
	this->file->popBlock();

	return result;
}

bool InternalNode::balanceLeafUnderflowRight(LeafNode& leftLeaf, LeafNode& rightLeaf, TreeBlock* underflowBlock)
{

	int totalCapacity = leftLeaf.getOccupiedSize() + rightLeaf.getOccupiedSize();
	if (totalCapacity < leftLeaf.getMaxSize())
	{
		// There is not enough records to balance the Leafs
		return false;
	}

	// remove first register from right child from parent
	VariableRecord aux;
	VariableRecord* keyAux;
	bool balanced = false;
	this->file->getCurrentBlock()->positionAtBegin();
	this->file->getCurrentBlock()->getNextRecord(&aux);
	keyAux = this->recordMethods->getKeyRecord(aux.getBytes(),aux.getSize());
	this->block->removeRecord(keyAux->getBytes());
	delete keyAux;


	VariableRecord* dataRecord;
	VariableRecord* keyRecord;


	while (leftLeaf.getOccupiedSize() < rightLeaf.getOccupiedSize())
	{
		balanced = true;
		dataRecord = rightLeaf.popFirst();
		keyRecord = this->recordMethods->getKeyRecord(dataRecord->getBytes(),dataRecord->getSize());

		underflowBlock->insertRecord(keyRecord, dataRecord);
		delete dataRecord;
		delete keyRecord;
	}
	if (balanced)
	{
		dataRecord = leftLeaf.popLast();
		keyRecord = this->recordMethods->getKeyRecord(dataRecord->getBytes(),
		dataRecord->getSize());
		this->file->getCurrentBlock()->insertRecord(keyRecord, dataRecord);
		underflowBlock->removeRecord(keyRecord->getBytes());
		delete dataRecord;
		delete keyRecord;
	}

	// update internal parent with first child of right node
	this->file->getCurrentBlock()->positionAtBegin();
	this->file->getCurrentBlock()->getNextRecord(&aux);
	keyAux = this->recordMethods->getKeyRecord(aux.getBytes(),aux.getSize());
	this->block->insertRecord(keyAux, keyAux);
	delete keyAux;

	return true;
}

bool InternalNode::balanceLeafUnderflowLeft(LeafNode& leftLeaf, LeafNode& rightLeaf,
		TreeBlock* underflowBlock, char* removedKey)
{
	int totalCapacity = leftLeaf.getOccupiedSize() + rightLeaf.getOccupiedSize();
	bool balanced = false;
	if (totalCapacity < leftLeaf.getMaxSize())
	{
		// There is not enough records to balance the Leafs
		return false;
	}

	// remove first register from right child from parent
	VariableRecord aux;
	VariableRecord* keyAux;
	underflowBlock->positionAtBegin();
	underflowBlock->getNextRecord(&aux);
	keyAux = this->recordMethods->getKeyRecord(aux.getBytes(),aux.getSize());
	if (this->recordMethods->compare(removedKey, aux.getBytes(), aux.getSize()) < 0)
	{
		this->block->removeRecord(removedKey);
	}
	else
	{
		this->block->removeRecord(keyAux->getBytes());
	}
	delete keyAux;

	VariableRecord* dataRecord;
	VariableRecord* keyRecord;

	while (leftLeaf.getOccupiedSize() > rightLeaf.getOccupiedSize())
	{
		balanced = true;
		dataRecord = leftLeaf.popLast();
		keyRecord = this->recordMethods->getKeyRecord(dataRecord->getBytes(),
				dataRecord->getSize());
		underflowBlock->insertRecord(keyRecord, dataRecord);
		delete dataRecord;
		delete keyRecord;
	}
	if (balanced)
	{
		dataRecord = rightLeaf.popFirst();
		keyRecord = this->recordMethods->getKeyRecord(dataRecord->getBytes(),
				dataRecord->getSize());
		this->file->getCurrentBlock()->insertRecord(keyRecord, dataRecord);
		underflowBlock->removeRecord(keyRecord->getBytes());
		delete dataRecord;
		delete keyRecord;
	}


	// update internal parent with first child of right node
	underflowBlock->positionAtBegin();
	underflowBlock->getNextRecord(&aux);
	keyAux = this->recordMethods->getKeyRecord(aux.getBytes(),aux.getSize());
	this->block->insertRecord(keyAux, keyAux);
	delete keyAux;

	return true;
}

int InternalNode::calculateMinimumSize()
{
	return floor((this->block->getSize() - SequenceTreeBlock::RECORD_OFFSET) * 0.45);
}

int InternalNode::getMinimumSize()
{
	return this->minimumSize;
}

OpResult InternalNode::update(char *key, VariableRecord *r)
{
	return Updated;
}

InternalNode::~InternalNode()
{
}
