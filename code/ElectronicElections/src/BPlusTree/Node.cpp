/*
 * Node.cpp
 *
 *  Created on: 20/09/2011
 *      Author: gonzalo
 */

#include "Node.h"

Node::Node(TreeBlock* tb, RecordMethods* methods) : recordMethods(methods), treeBlock(tb)
{
}

Node::~Node()
{
}

