
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
#include <string.h>

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

void Tree::changeBlock(int newBlock2, VariableRecord* middleRecord)
{
    this->file->saveBlock();
    this->file->popBlock();
    this->file->loadBlock(newBlock2);
    this->file->pushBlock();
    this->file->getCurrentBlock()->forceInsert(middleRecord);
    this->file->getCurrentBlock()->setNextNode(0);
    this->file->getCurrentBlock()->setLevel(0);
}

void Tree::handleOverflowInLeafRoot(VariableRecord *dataRecord)
{
    TreeBlock *currentBlock = this->file->getCurrentBlock();
    bool blockChanged = false;
    int newBlock1 = this->file->getFreeBlockManager().getNextBlock();
    int newBlock2 = this->file->getFreeBlockManager().getNextBlock();
    this->file->loadBlock(newBlock1);
    this->file->pushBlock();
    if (!this->file->isCurrentLeaf())
	{
		this->file->swapBlockKind();
	}
	else
	{
		this->file->getCurrentBlock()->clear();
	}
    this->file->getCurrentBlock()->setNextNode(newBlock2);
    this->file->getCurrentBlock()->setLevel(0);
    VariableRecord aux;
    currentBlock->positionAtBegin();
    VariableRecord* keyAux = this->methods->getKeyRecord(dataRecord->getBytes(), dataRecord->getSize());
    while(currentBlock->getNextRecord(&aux) != NULL)
	{
		int result = this->methods->compare(keyAux->getBytes(), aux.getBytes(), aux.getSize());
		if (result > 0)
		{
			// first leaf
			this->file->getCurrentBlock()->forceInsert(&aux);
		}
		else
		{
			// second leaf
			if (!blockChanged)
			{
				blockChanged = true;
			    this->changeBlock(newBlock2, dataRecord);
			}
			this->file->getCurrentBlock()->forceInsert(&aux);
		}
	}

    delete keyAux;

	if (!blockChanged)
	{
		blockChanged = true;
		this->changeBlock(newBlock2, dataRecord);
	}

    this->file->saveBlock();
    this->file->popBlock();

    this->file->swapBlockKind();
    this->file->getCurrentBlock()->insertNodePointer(0, newBlock1);
    VariableRecord* middleKeyRecord = this->methods->getKeyRecord(dataRecord->getBytes(), dataRecord->getSize());
    this->file->getCurrentBlock()->insertRecord(middleKeyRecord, middleKeyRecord);
    this->file->getCurrentBlock()->insertNodePointer(1, newBlock2);
    this->file->getCurrentBlock()->setLevel(1);

    delete middleKeyRecord;

    this->file->saveBlock();

    delete this->root;
    this->root = new InternalNode(this->file,
    		this->file->getCurrentBlock(), this->methods);
}

void Tree::handleOverflowInInternalRoot(VariableRecord *dataRecord, OverflowParameter& overflowParameter)
{
	TreeBlock* rootBlock = this->file->getCurrentBlock();
	int newBlock1 = this->file->getFreeBlockManager().getNextBlock();
	int newBlock2 = this->file->getFreeBlockManager().getNextBlock();
	this->file->loadBlock(newBlock1);
	this->file->pushBlock();
	if (this->file->isCurrentLeaf())
	{
		this->file->swapBlockKind();
	}
	else
	{
		this->file->getCurrentBlock()->clear();
	}
	this->file->getCurrentBlock()->setLevel(this->root->getLevel());

	int index = 0;
	VariableRecord aux;
	int nodePointer = -1;

	rootBlock->positionAtBegin();
	while (rootBlock->getNextRecord(&aux) != NULL &&
			this->methods->compare(dataRecord->getBytes(), aux.getBytes(), aux.getSize()) > 0)
	{
		nodePointer = rootBlock->getNodePointer(index);
		this->file->getCurrentBlock()->insertNodePointer(index, nodePointer);
		this->file->getCurrentBlock()->forceInsert(&aux);
		index++;
	}

	nodePointer = rootBlock->getNodePointer(index);
	this->file->getCurrentBlock()->insertNodePointer(index, nodePointer);
	index++;

	this->file->saveBlock();
	this->file->popBlock();
	this->file->loadBlock(newBlock2);
	this->file->pushBlock();
	this->file->swapBlockKind();

	int index2 = 0;
	this->file->getCurrentBlock()->insertNodePointer(index2, overflowParameter.newBlock);
	index2++;

	do
	{
		nodePointer = rootBlock->getNodePointer(index);
		this->file->getCurrentBlock()->insertNodePointer(index2, nodePointer);
		this->file->getCurrentBlock()->forceInsert(&aux);
		index++;
		index2++;
	}while (rootBlock->getNextRecord(&aux) != NULL);

	this->file->getCurrentBlock()->setLevel(this->root->getLevel());
	this->file->saveBlock();
	this->file->popBlock();

	rootBlock->clear();
	rootBlock->insertNodePointer(0, newBlock1);
	rootBlock->insertNodePointer(1, newBlock2);
	rootBlock->forceInsert(dataRecord);
	this->root->increaseLevel();
	this->file->saveBlock();
}

OpResult Tree::insert(VariableRecord *keyRecord, VariableRecord *dataRecord)
{
	OverflowParameter overflowParameter;
	overflowParameter.newBlock = -1;
	overflowParameter.overflowBlock = -1;
	OpResult result = this->root->insert(keyRecord, dataRecord, overflowParameter);

	// overflow in root, split
	if (result == Overflow)
	{
		if (this->file->isCurrentLeaf())
		{
			this->handleOverflowInLeafRoot(dataRecord);
		}
		else
		{
			this->handleOverflowInInternalRoot(dataRecord, overflowParameter);
		}
	}

	this->file->saveBlock();
	return result;
}

void Tree::decreaseTreeHeight()
{
	int nodePointer = this->file->getCurrentBlock()->getNodePointer(0);
	this->file->getFreeBlockManager().addFreeBlock(nodePointer);
	this->file->loadBlock(nodePointer);
	this->file->pushBlock();
	int childLevel = this->file->getCurrentBlock()->getLevel();
	int blockSize = this->file->getCurrentBlock()->getSize();
	char buffer[blockSize];
	memcpy(buffer, this->file->getCurrentBlock()->getBytes(), blockSize);
	this->file->popBlock();

	if (childLevel == 0)
	{
		this->file->swapBlockKind();
	}

	TreeBlock* rootBlock = this->file->getCurrentBlock();
    memcpy(rootBlock->getBytes(), buffer, blockSize);
    rootBlock->updateInformation();
    if(rootBlock->getLevel() == 0)
    {
        delete this->root;
        this->root = new LeafNode(rootBlock, methods);
    }
    this->file->saveBlock();
}

OpResult Tree::remove(char *key)
{
	OpResult result = this->root->remove(key);
	VariableRecord aux;
	this->file->getCurrentBlock()->positionAtBegin();
	int i = 0;
	while(this->file->getCurrentBlock()->getNextRecord(&aux)){i++;};
	if (result == Underflow && !this->file->isCurrentLeaf() && i == 0)
	{
	    this->decreaseTreeHeight();
	    result = Updated;
	}

	return result;
}

OpResult Tree::update(char *key, VariableRecord *r)
{
	OverflowParameter overflowParameter;
	OpResult result = this->root->update(key, r, overflowParameter);

	// overflow in root, split
	if (result == Overflow)
	{
		if (this->file->isCurrentLeaf())
		{
			this->handleOverflowInLeafRoot(r);
		}
		else
		{
			this->handleOverflowInInternalRoot(r, overflowParameter);
		}
	}

	this->file->saveBlock();
	return result;
}

bool Tree::get(char* key, VariableRecord* r)
{
	this->file->deleteKeptBlock();
	bool equal = this->root->get(key, r);
	if (this->file->isCurrentLeaf())
	{
		this->file->setKeptBlock(this->file->getCurrentBlock());
	}
	if (r->getSize() != 0)
	{
		VariableRecord* keyRecord = this->methods->getKeyRecord(r->getBytes(), r->getSize());
		this->lastKey.setBytes(keyRecord->getBytes(),keyRecord->getSize());
		delete keyRecord;
	}

	return equal;
}

VariableRecord* Tree::getNext(VariableRecord* r)
{
	// Check if key is in currentBlock or next
	VariableRecord aux;
	if (this->file->getKeptBlock() == NULL)
	{
		this->returnFirst(r);
		return r;
	}
	this->file->getKeptBlock()->positionAtBegin();
	while (this->file->getKeptBlock()->getNextRecord(&aux) != NULL){}
	if (this->methods->compare
			(this->lastKey.getBytes(), aux.getBytes(), aux.getSize()) >= 0 )
	{
		// Load next block
		int nextNode = this->file->getKeptBlock()->getNextNode();
		if (nextNode == 0)
		{
			r = NULL;
			return r;
		}
		this->file->loadBlock(nextNode);
		this->file->pushBlock();
		this->file->deleteKeptBlock();
		this->file->popAndKeep();
	}
	this->file->getKeptBlock()->positionAtBegin();
	while (this->file->getKeptBlock()->getNextRecord(&aux) != NULL)
	{
		if (this->methods->compare
					(this->lastKey.getBytes(), aux.getBytes(), aux.getSize()) < 0 )
		{
			r->setBytes(aux.getBytes(),aux.getSize());
			this->lastKey.setBytes(aux.getBytes(),aux.getSize());
			return r;
		}
	}
	return r;
}

VariableRecord* Tree::returnFirst(VariableRecord* r)
{
	this->root->returnFirst(r);
	this->lastKey.setBytes(r->getBytes(),r->getSize());
	return r;
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
