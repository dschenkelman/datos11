/*
 * Node.cpp
 *
 *  Created on: 20/09/2011
 *      Author: gonzalo
 */

#include "Node.h"

Node::Node(TreeBlock* b, RecordMethods* methods):block(b), recordMethods(methods)
{
}

short Node::getLevel()
{
	return this->block->getLevel();
}

void Node::increaseLevel()
{
	return this->block->setLevel(this->block->getLevel() + 1);
}

Node::~Node()
{
}

