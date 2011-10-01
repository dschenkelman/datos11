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

using namespace std;

InternalNode::InternalNode(TreeBlockFile* f, TreeBlock* b, RecordMethods* methods): Node(b, methods), file(f)
{
}

OpResult InternalNode::insert(VariableRecord *keyRecord, VariableRecord *dataRecord, VariableRecord* middleRecord)
{
	VariableRecord aux;
	this->block->positionAtBegin();
	int index;
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

	if (this->file->isCurrentLeaf())
	{
		LeafNode node(this->file->getCurrentBlock(), this->recordMethods);
		node.insert(keyRecord, dataRecord, middleRecord);
	}
	else
	{
		InternalNode node(this->file, this->file->getCurrentBlock(), this->recordMethods);
		node.insert(keyRecord, dataRecord, middleRecord);
	}

	// save block


	// restore block
	this->file->popBlock();

	return Updated;
}

void InternalNode::print()
{
	VariableRecord aux;
	this->block->positionAtBegin();
	int i = 0;
	vector<int> nodes;
	while(this->block->getNextRecord(&aux) != NULL)
	{
		nodes.push_back(i);
		cout << this->block->getNodePointer(i) << endl;
		i++;
		this->recordMethods->printKey(aux.getBytes(), aux.getSize());
	}

	nodes.push_back(i);
	cout << this->block->getNodePointer(i) << endl;

	while(nodes.size() != 0)
	{
		int index = nodes.at(nodes.size()  - 1);
		nodes.pop_back();
		int blockPointer = this->block->getNodePointer(index);
		this->file->loadBlock(blockPointer);
		this->file->pushBlock();
		if (this->file->isCurrentLeaf())
		{
			LeafNode node(this->file->getCurrentBlock(), this->recordMethods);
			node.print();
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
