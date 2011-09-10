/*
 * Block.h
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#ifndef BLOCK_H_
#define BLOCK_H_

// represents a block. format:
// B(contentSize(4 bytes), (registerSize(4 bytes), registerContent)*)
class Block {
	// max size of the block [bytes]
	int maxSize;
	// occupied size of the block [bytes]
	int freeSpace;
	// content of the block
	char* bytes;
public:
	Block(int size);
	char* getBytes();
	int getFreeSpace();
	virtual ~Block();
};

#endif /* BLOCK_H_ */
