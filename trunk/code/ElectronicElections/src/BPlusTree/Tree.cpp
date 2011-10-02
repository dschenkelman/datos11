
/*
 * Tree.cpp
 *
 *  Created on: Sep 30, 2011
 *      Author: damian
 */

#include "Tree.h"
#include <fstream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "IndexTreeBlock.h"

using namespace std;

Tree::Tree(string fileName, int blockSize, RecordMethods* methods, bool createNew = false)
: methods(methods)
{
	if (!createNew && !ifstream(fileName.c_str(), ifstream::in))
	{
		createNew = true;
	}

	this->file = new TreeBlockFile(fileName, blockSize, methods, createNew);
	// load root
	this->file->loadBlock(0);
	this->file->pushBlock();
	if (createNew)
	{
		this->root = new LeafNode(this->file->getCurrentBlock(), methods);
		this->file->saveBlock();
	}
	else
	{
		if (this->file->isCurrentLeaf())
		{
			this->root = new LeafNode(this->file->getCurrentBlock(), methods);
		}
		else
		{
			this->root = new InternalNode(this->file, this->file->getCurrentBlock(), methods);
		}
	}
}

void Tree::handleOverflowInLeafRoot(VariableRecord *keyRecord, VariableRecord& middleRecord, VariableRecord *dataRecord)
{
    TreeBlock *currentBlock = this->file->getCurrentBlock();
    bool blockOne = true;
    if(this->methods->compare(keyRecord->getBytes(), middleRecord.getBytes(), middleRecord.getSize()) > 0){
        blockOne = false;
    }
    int newBlock1 = this->file->getFreeBlockManager().getNextBlock();
    int newBlock2 = this->file->getFreeBlockManager().getNextBlock();
    this->file->loadBlock(newBlock1);
    this->file->pushBlock();
    this->file->getCurrentBlock()->setNextNode(newBlock2);
    this->file->getCurrentBlock()->setLevel(0);
    VariableRecord aux;
    currentBlock->positionAtBegin();
    while(currentBlock->getNextRecord(&aux) != NULL)
	{
		int result = this->methods->compare(middleRecord.getBytes(), aux.getBytes(), aux.getSize());
		if (result > 0)
		{
			// first leaf
			this->file->getCurrentBlock()->forceInsert(&aux);
		}
		else if (result == 0)
		{
			if (blockOne)
			{
				this->file->getCurrentBlock()->insertRecord(keyRecord, dataRecord);
			}

			// second leaf
			this->file->saveBlock();
			this->file->popBlock();
			this->file->loadBlock(newBlock2);
			this->file->pushBlock();
			this->file->getCurrentBlock()->forceInsert(&aux);
			this->file->getCurrentBlock()->setNextNode(0);
			this->file->getCurrentBlock()->setLevel(0);
		}
		else
		{
			this->file->getCurrentBlock()->forceInsert(&aux);
		}
	}

    if(!blockOne)
    {
        this->file->getCurrentBlock()->insertRecord(keyRecord, dataRecord);
    }
    this->file->saveBlock();
    this->file->popBlock();

    this->file->swapBlockKind();
    this->file->getCurrentBlock()->insertNodePointer(0, newBlock1);
    VariableRecord* middleKeyRecord =
    		this->methods->getKeyRecord(middleRecord.getBytes(), middleRecord.getSize());
    this->file->getCurrentBlock()->insertRecord(middleKeyRecord, middleKeyRecord);
    this->file->getCurrentBlock()->insertNodePointer(1, newBlock2);
    this->file->getCurrentBlock()->setLevel(1);

    delete middleKeyRecord;

    this->file->saveBlock();

    delete this->root;
    this->root = new InternalNode(this->file,
    		this->file->getCurrentBlock(), this->methods);
}

void Tree::handleOverflowInInternalRoot(VariableRecord *keyRecord,
		VariableRecord & middleRecord, VariableRecord *dataRecord, OverflowParameter& overflowParameter)
{
	TreeBlock* rootBlock = this->file->getCurrentBlock();
	int newBlock1 = this->file->getFreeBlockManager().getNextBlock();
	int newBlock2 = this->file->getFreeBlockManager().getNextBlock();
	this->file->loadBlock(newBlock1);
	this->file->pushBlock();
	this->file->swapBlockKind();
	this->file->getCurrentBlock()->setLevel(this->root->getLevel());

	int size = 0;
	int index = 0;
	VariableRecord aux;
	int nodePointer = -1;
	int extra = 0;
	bool overflowBlockIncluded = false;

	rootBlock->positionAtBegin();
	while (rootBlock->getNextRecord(&aux) != NULL)
	{
		size += aux.getSize() + IndexTreeBlock::NODE_POINTER_SIZE + Constants::RECORD_HEADER_SIZE;
		if (size > this->root->getMaxSize() / 2)
		{
			break;
		}

		nodePointer = rootBlock->getNodePointer(index);
		this->file->getCurrentBlock()->insertNodePointer(index + extra, nodePointer);
		index++;
		if (nodePointer == overflowParameter.overflowBlock)
		{
			size += middleRecord.getSize() + Constants::RECORD_HEADER_SIZE + IndexTreeBlock::NODE_POINTER_SIZE;

			if (size > this->root->getMaxSize() / 2)
			{
				break;
			}

			this->file->getCurrentBlock()->insertNodePointer(index, overflowParameter.newBlock);
			extra = 1;
			this->file->getCurrentBlock()->forceInsert(&middleRecord);
			overflowBlockIncluded = true;
		}

		this->file->getCurrentBlock()->forceInsert(&aux);
	}

	// skip middle record
	VariableRecord keepInParent = aux;
	nodePointer = rootBlock->getNodePointer(index);
	this->file->getCurrentBlock()->insertNodePointer(index + extra, nodePointer);
	rootBlock->getNextRecord(&aux);
	index++;

	this->file->saveBlock();
	this->file->popBlock();
	this->file->loadBlock(newBlock2);
	this->file->pushBlock();
	this->file->swapBlockKind();

	int index2 = 0;

	do
	{
		int nodePointer = rootBlock->getNodePointer(index);
		this->file->getCurrentBlock()->insertNodePointer(index2, nodePointer);
		if (nodePointer == overflowParameter.overflowBlock)
		{
			this->file->getCurrentBlock()->forceInsert(&middleRecord);
			this->file->getCurrentBlock()->insertNodePointer(++index2, overflowParameter.newBlock);
		}
		this->file->getCurrentBlock()->forceInsert(&aux);
		index++;
		index2++;
	}while (rootBlock->getNextRecord(&aux) != NULL);

	nodePointer = rootBlock->getNodePointer(index);
	this->file->getCurrentBlock()->insertNodePointer(index2, nodePointer);
	this->file->getCurrentBlock()->setLevel(this->root->getLevel());
	this->file->saveBlock();
	this->file->popBlock();

	rootBlock->clear();
	rootBlock->insertNodePointer(0, newBlock1);
	rootBlock->insertNodePointer(1, newBlock2);
	rootBlock->forceInsert(&keepInParent);
	this->root->increaseLevel();
	this->file->saveBlock();
}

OpResult Tree::insert(VariableRecord *keyRecord, VariableRecord *dataRecord)
{
	VariableRecord middleRecord;
	OverflowParameter overflowParameter;
	overflowParameter.newBlock = -1;
	overflowParameter.overflowBlock = -1;
	OpResult result = this->root->insert(keyRecord, dataRecord, &middleRecord, overflowParameter);

	// overflow in root, split
	if (result == Overflow)
	{
		if (this->file->isCurrentLeaf())
		{
			this->handleOverflowInLeafRoot(keyRecord, middleRecord, dataRecord);
		}
		else
		{
			this->handleOverflowInInternalRoot(keyRecord, middleRecord, dataRecord, overflowParameter);
		}
	}

	this->file->saveBlock();
	return result;
}

OpResult Tree::remove(char *key)
{
	return this->root->remove(key);
}

OpResult Tree::update(char *key, VariableRecord *r)
{
	return this->root->update(key, r);
}

void Tree::print()
{
	this->root->print();
}

Tree::~Tree()
{
	delete this->root;
	delete this->file;
}
