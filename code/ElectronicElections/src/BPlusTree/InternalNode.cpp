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
	return this->maximumSize;
}

int InternalNode::calculateMaxSize()
{
	return floor((this->block->getSize() - IndexTreeBlock::RECORD_OFFSET)* 0.9);
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
    if (!this->file->isCurrentLeaf())
    {
    	this->file->swapBlockKind();
    }
    else
    {
    	this->file->getCurrentBlock()->clear();
    }
    this->file->getCurrentBlock()->setNextNode(newNextNode);
    this->file->getCurrentBlock()->setLevel(0);
    bool middleInserted = false;
    VariableRecord aux;
    VariableRecord* keyAux = this->recordMethods->getKeyRecord(dataRecord->getBytes(), dataRecord->getSize());
    stack<VariableRecord*> toRemove;
    oldLeafBlock->positionAtBegin();
    while(oldLeafBlock->getNextRecord(&aux) != NULL)
	{
		int result = this->recordMethods->compare
				(keyAux->getBytes(), aux.getBytes(), aux.getSize());
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

    delete keyAux;

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
    if (this->file->isCurrentLeaf())
	{
       	this->file->swapBlockKind();
	}
    else
	{
		this->file->getCurrentBlock()->clear();
	}
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
		bool *leafAlreadyBalanced, LeafNode& node, char* key, int index)
{
    TreeBlock *underflowBlock = this->file->getCurrentBlock();
    this->file->loadBlock(nextNode);
    this->file->pushBlock();
    LeafNode brother(this->file->getCurrentBlock(), this->recordMethods);
    if(!lastChild)
    {
    	*leafAlreadyBalanced = this->balanceLeafUnderflowRight(node, brother, underflowBlock);
    }
    else
    {
    	*leafAlreadyBalanced = this->balanceLeafUnderflowLeft(brother, node, underflowBlock, key);
    }


    if(!(*leafAlreadyBalanced))
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

void InternalNode::mergeLeafNodes(int index, TreeBlock *brotherBlock, TreeBlock *underflowBlock,
		char* removedKey, bool lastChild)
{
    // InternalNode Stuff
	VariableRecord aux;
    VariableRecord *keyAux;
    this->file->getFreeBlockManager().addFreeBlock(this->block->getNodePointer(index + 1));
    this->block->removeNodePointer(index + 1);
    brotherBlock->positionAtBegin();
    brotherBlock->getNextRecord(&aux);
    keyAux = this->recordMethods->getKeyRecord(aux.getBytes(), aux.getSize());
    VariableRecord previous;
	VariableRecord current;
	this->block->positionAtBegin();
	while(this->block->getNextRecord(&current))
	{
		if (this->recordMethods->compare(keyAux->getBytes(), current.getBytes(), current.getSize()) < 0)
		{
			break;
		}
		previous = current;
	}

    delete keyAux;

	if (previous.getBytes() != NULL)
	{
		keyAux = this->recordMethods->getKeyRecord(previous.getBytes(), previous.getSize());
		this->block->removeRecord(keyAux->getBytes());
		delete keyAux;
	}
	else
	{
		keyAux = this->recordMethods->getKeyRecord(current.getBytes(), current.getSize());
		this->block->removeRecord(keyAux->getBytes());
		delete keyAux;
	}



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



bool InternalNode::balanceInternalNodeToTheLeft(TreeBlock *underflowBlock, TreeBlock *balancingBlock, VariableRecord & aux)
{
	int totalOccupied = balancingBlock->getOccupiedSize() + underflowBlock->getOccupiedSize() - 2 * IndexTreeBlock::RECORD_OFFSET;
	InternalNode balancingNode(this->file, balancingBlock, this->recordMethods);
	if((totalOccupied < balancingNode.getMaxSize()))
	{
		return false;
	}
	int underflowRecordCount = 0;
	VariableRecord aux2;
	underflowBlock->positionAtBegin();
	while (underflowBlock->getNextRecord(&aux2)){ underflowRecordCount++; }
    while(underflowBlock->getOccupiedSize() < balancingBlock->getOccupiedSize())
    {
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
	return true;
}

bool InternalNode::balanceInternalNodeToTheRight(TreeBlock *balancingBlock, TreeBlock *underflowBlock)
{
	int totalOccupied = balancingBlock->getOccupiedSize() + underflowBlock->getOccupiedSize() - 2 * IndexTreeBlock::RECORD_OFFSET;
	InternalNode balancingNode(this->file, balancingBlock, this->recordMethods);
	if((totalOccupied < balancingNode.getMaxSize()))
	{
		return false;
	}

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
            return true;
        }
        underflowBlock->insertRecord(lastParentRecord, lastParentRecord);
        int nodePointer = balancingBlock->getNodePointer(balanceRecordCount);
        balancingBlock->removeNodePointer(balanceRecordCount);
        VariableRecord *lastBalancingRecord = balancingBlock->popLast();
        balanceRecordCount--;
        underflowBlock->insertNodePointer(0, nodePointer);
        this->block->insertRecord(lastBalancingRecord, lastBalancingRecord);
        delete lastParentRecord;
        delete lastBalancingRecord;
    }
    return true;
}

void InternalNode::mergeNodeToTheLeft(VariableRecord & nextKeyInFather, int index, TreeBlock *underflowBlock, TreeBlock *balancingBlock)
{
	VariableRecord aux;
	VariableRecord *keyAux;
    keyAux = this->recordMethods->getKeyRecord(nextKeyInFather.getBytes(), nextKeyInFather.getSize());
    this->block->removeRecord(keyAux->getBytes());
    this->file->getFreeBlockManager().addFreeBlock(this->block->getNodePointer(index + 1));
    this->block->removeNodePointer(index + 1);
    underflowBlock->insertRecord(keyAux, keyAux);
    delete keyAux;
    int underflowIndex = 0;
    underflowBlock->positionAtBegin();
    while(underflowBlock->getNextRecord(&aux) != NULL)
					{
						underflowIndex++;
					}
    int nodePointer = balancingBlock->getNodePointer(0);
    balancingBlock->removeNodePointer(0);
    underflowBlock->insertNodePointer(underflowIndex, nodePointer);
    underflowIndex++;
    while(!balancingBlock->isEmpty()){
        VariableRecord *firstParentRecord = balancingBlock->popFirst();
        underflowBlock->forceInsert(firstParentRecord);
        delete firstParentRecord;
        nodePointer = balancingBlock->getNodePointer(0);
        balancingBlock->removeNodePointer(0);
        underflowBlock->insertNodePointer(underflowIndex, nodePointer);
        underflowIndex++;
    }
}

void InternalNode::mergeLastInternalChildNode(VariableRecord & lastRecord, int index, TreeBlock *balancingBlock, TreeBlock *underflowBlock)
{
    VariableRecord aux;
    this->block->removeRecord(lastRecord.getBytes());
    this->file->getFreeBlockManager().addFreeBlock(this->block->getNodePointer(index));
    this->block->removeNodePointer(index);
    balancingBlock->insertRecord(&lastRecord, &lastRecord);
    int balancingIndex = 0;
    balancingBlock->positionAtBegin();
    while(balancingBlock->getNextRecord(&aux) != NULL)
	{
		balancingIndex++;
	}
    int nodePointer = underflowBlock->getNodePointer(0);
    underflowBlock->removeNodePointer(0);
    balancingBlock->insertNodePointer(balancingIndex, nodePointer);
    balancingIndex++;
    while(!underflowBlock->isEmpty()){
        VariableRecord *firstParentRecord = underflowBlock->popFirst();
        balancingBlock->forceInsert(firstParentRecord);
        delete firstParentRecord;
        nodePointer = underflowBlock->getNodePointer(0);
        underflowBlock->removeNodePointer(0);
        balancingBlock->insertNodePointer(balancingIndex, nodePointer);
        balancingIndex++;
    }
}

OpResult InternalNode::remove(char *key)
{
	VariableRecord aux;
	VariableRecord lastRecord;
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

		lastRecord = aux;
		index++;
	}
	//To be used in internalNode merging
	VariableRecord nextKeyInFather = aux;

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
				&leafAlreadyBalanced, node, key, index);

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
			bool balanced = false;
		    if (lastChild)
		    {
		    	balanced = this->balanceInternalNodeToTheRight(balancingBlock, underflowBlock);
    		}
		    else
		    {
		    	balanced = this->balanceInternalNodeToTheLeft(underflowBlock, balancingBlock, aux);
		    }

			result = Updated;

		    if (!balanced)
		    {
		    	if (!lastChild)
		    	{
	    		    this->mergeNodeToTheLeft(nextKeyInFather, index, underflowBlock, balancingBlock);
	    		}
		    	else
		    	{
	    		    this->mergeLastInternalChildNode(lastRecord, index, balancingBlock, underflowBlock);
	    		}


    		    if ((this->block->getOccupiedSize() - IndexTreeBlock::RECORD_OFFSET) < this->minimumSize)
				{
    		    	result = Underflow;
				}
		    }

		    this->file->saveBlock();
			this->file->popBlock();
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

	int totalCapacity = leftLeaf.getOccupiedSize() + rightLeaf.getOccupiedSize() - 2 * SequenceTreeBlock::RECORD_OFFSET;
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
    VariableRecord previous;
	VariableRecord current;
	this->block->positionAtBegin();
	while(this->block->getNextRecord(&current))
	{
		if (this->recordMethods->compare(keyAux->getBytes(), current.getBytes(), current.getSize()) < 0)
		{
			break;
		}
		previous = current;
	}

	this->block->removeRecord(previous.getBytes());
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
	int totalCapacity = leftLeaf.getOccupiedSize() + rightLeaf.getOccupiedSize() - 2 * SequenceTreeBlock::RECORD_OFFSET;
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
    VariableRecord previous;
	VariableRecord current;
	this->block->positionAtBegin();
	while(this->block->getNextRecord(&current))
	{
		if (this->recordMethods->compare(keyAux->getBytes(), current.getBytes(), current.getSize()) < 0)
		{
			break;
		}
		previous = current;
	}

	this->block->removeRecord(previous.getBytes());
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

bool InternalNode::isUnderflow()
{
	return (this->block->getOccupiedSize() - IndexTreeBlock::RECORD_OFFSET) < this->minimumSize;
}

bool InternalNode::get(char* key, VariableRecord* record, TreeBlock** currentLeafBlock)
{
	VariableRecord aux;
	bool result;
	int index = 0;
	this->block->positionAtBegin();
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
		LeafNode leaf(this->file->getCurrentBlock(), this->recordMethods);
		bool found = leaf.get(key, record, currentLeafBlock);
		*currentLeafBlock = this->file->popAndKeep();
		return found;
	}

	// Internal Node
	InternalNode internal(this->file, this->file->getCurrentBlock(), this->recordMethods);
	result = internal.get(key, record, currentLeafBlock);
	this->file->popBlock();
	return result;

}

VariableRecord* InternalNode::returnFirst(VariableRecord* r, TreeBlock** currentLeafBlock)
{
	VariableRecord aux;
	bool result;
	int index = 0;

	int blockPointer = this->block->getNodePointer(index);
	this->file->loadBlock(blockPointer);
	this->file->pushBlock();

	if (this->file->isCurrentLeaf())
	{
		LeafNode leaf(this->file->getCurrentBlock(), this->recordMethods);
		leaf.returnFirst(r, currentLeafBlock);
		*currentLeafBlock = this->file->popAndKeep();
		return r;
	}

	// Internal Node
	InternalNode internal(this->file, this->file->getCurrentBlock(), this->recordMethods);
	result = internal.returnFirst(r, currentLeafBlock);
	this->file->popBlock();
	return r;
}

InternalNode::~InternalNode()
{
}
