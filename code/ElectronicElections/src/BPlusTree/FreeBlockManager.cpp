/*
 * FreeBlockManager.cpp
 *
 *  Created on: Sep 30, 2011
 *      Author: damian
 */

#include "FreeBlockManager.h"
#include <string.h>

using namespace std;

FreeBlockManager::FreeBlockManager(std::string fName, bool createNew)
{
	ios_base::openmode mode = ios::binary | ios::in | ios::out;
	if (createNew)
	{
		mode = mode | ios::trunc;
	}
	this->file.open(fName.c_str(), mode);
	if (createNew)
	{
		char buffer[2 * sizeof(int)];
		int value = 1;
		memcpy(buffer, &value, sizeof(int));
		memcpy(buffer + sizeof(int), &value, sizeof(int));
		this->file.clear();
		this->file.seekp(0, ios::beg);
		this->file.write(buffer, 2 * sizeof(int));
	}
	this->loadFreeBlocks();
}

void FreeBlockManager::loadFreeBlocks()
{
	this->file.seekg(0, ios::end);
	int size = this->file.tellg();
	this->file.seekg(0, ios::beg);
	char buffer[size];
	this->file.read(buffer, size);
	memcpy(&this->size, buffer, sizeof(int));

	int value;
	for (int i = 1; i <= this->size; i++)
	{
		memcpy(&value, buffer + i * sizeof(int), sizeof(int));
		this->blocks.push_back(value);
	}
}

int FreeBlockManager::getNextBlock()
{
	int value = this->blocks.at(this->blocks.size() - 1);
	int value2 = value;
	if (this->blocks.size() != 1)
	{
		this->blocks.pop_back();
	}
	else
	{
		this->blocks[this->blocks.size() - 1] = ++value2;
	}

	this->size--;
	this->saveFreeBlocks();

	return value;
}

void FreeBlockManager::saveFreeBlocks()
{
	this->file.clear();
	this->file.seekp(0, ios::beg);
    char buffer[this->size + 1];
    memcpy(buffer, &this->size, sizeof (int));
    for(int i = 1; i <= this->size; i++)
    {
        memcpy(buffer + i * sizeof (int), &this->blocks[i], sizeof (int));
    }
    this->file.write(buffer, (this->size + 1) * sizeof (int));
}

int FreeBlockManager::addFreeBlock(int block)
{
	this->blocks.push_back(block);
	this->size++;
    this->saveFreeBlocks();
}

FreeBlockManager::~FreeBlockManager()
{
	this->file.close();
}
