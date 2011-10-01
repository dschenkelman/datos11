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

OpResult Tree::insert(VariableRecord *keyRecord, VariableRecord *dataRecord)
{
	VariableRecord middleRecord;
	OpResult result = this->root->insert(keyRecord, dataRecord, &middleRecord);

	// overflow in root, split
	if (result == Overflow && this->file->isCurrentLeaf())
	{
	    this->handleOverflowInLeafRoot(keyRecord, middleRecord, dataRecord);
	}
	else
	{

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
