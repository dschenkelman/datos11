/*
 * Block.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: damian
 */

#include "Block.h"
#include <string.h>
#include <exception>
#include "Constants.h"

Block::Block(int blockSize, recordSize, RecordMethods* methods) : maxSize(blockSize),
position(Constants::BLOCK_HEADER_SIZE), recordSize(recordSize)
{
	this->bytes = new char[this->maxSize];
	memset(this->bytes, 0, this->maxSize);
	this->occupiedRecords = 0;
	this->recordMethods = methods;
	this->recordCount = this->maxSize;
}

Block& Block::operator =(const Block & other)
{
	if (this == &other)
	{
		return *this;
	}

	this->copyBlock(other);

	return *this;
}

Block::Block(Block& other)
{
    this->copyBlock(other);
}

/*char* Block::getBytes()
{
	return this->bytes;
}*/

bool Block::isOverflowed()
{
	return this->overflow;
}
void Block::getsOverflow()
{
	this->overflow = true;
}

int Block::getRecordCount()
{
	return this->recordCount;
}

void Block::updateInformation() //ESTO SIRVE? 
	//A LA HORA DE INSERTAR O BORRAR, SE ACTUALIZAN LOS FLAGS 
	//DE CONTEO Y LISTO.
{
	int occupiedSize;
	memcpy(&occupiedSize, this->bytes, Constants::BLOCK_HEADER_SIZE);
	this->freeSpace = this->maxSize - occupiedSize;

    // first four bytes in each record represent record size

	// ignore the first four bytes
	int sum = Constants::BLOCK_HEADER_SIZE;
	int records = 0;
	while(sum < occupiedSize)
	{
		int recordSize;
		memcpy(&recordSize, this->bytes + sum, Constants::RECORD_HEADER_SIZE);
		sum += (recordSize + Constants::RECORD_HEADER_SIZE);
		records++;
	}

	this->recordCount = records;
}

bool isEmpty()
{
	return (this->occupiedRecords == 0);
}
bool isFull()
{
	return (this->occupiedRecords == this->recordCount);
}

inline int Block::getOccupiedRecordNumbers()
{
	return this->occupiedRecords;
}

bool Block::hasNextRecord()
{
	return this->currentRecord < this->recordCount;
}

Record* Block::getNextRecord(Record* r)
{
	if (!this->hasNextRecord())
	{
		return NULL;
	}

	//ME OLVIDE COMO HACER EL JUEGO DE LOS FLAGS DE LOS RECORDS,
	//	SACANDO ESE OFFSET
	/*int recordSize;
	memcpy(&recordSize, this->bytes + this->position, Constants::RECORD_HEADER_SIZE);
	this->position += Constants::RECORD_HEADER_SIZE;
	r->setBytes(this->bytes + this->position, recordSize);
	this->position += recordSize;
	*/
	return r;
}
Record* Block::getRecord(Record* r)
{
	//ME OLVIDE COMO HACER EL JUEGO DE LOS FLAGS DE LOS RECORDS,
	//	SACANDO ESE OFFSET
	/*int recordSize;
	memcpy(&recordSize, this->bytes + this->position, Constants::RECORD_HEADER_SIZE);
	this->position += Constants::RECORD_HEADER_SIZE;
	r->setBytes(this->bytes + this->position, recordSize);
	this->position += recordSize;
	*/
	return r;
}

int Block::findRecord(const char* key, Record** rec)
{
	//this->position = Constants::BLOCK_HEADER_SIZE;  VERIFICAR LOS PRIMEROS FLAGS??
	Record* record = new Record();
	this->seek(0); //comienzo del 1er registro a buscar
	int foundRecord = this->currentRecord;
	while(this->getNextRecord(record) != NULL)
	{
		if (this->recordMethods->compare(key,
				record->getBytes(), record->getSize()) == 0)
		{
			*rec = record;
			return foundPosition;
		}
		delete record;
		record = new Record();
		foundPosition = this->currentRecord;
	}

	delete record;
	return -1;
}

void Block::clear()
{
	memset(this->bytes, 0, this->maxSize);
	// FALTA SETEO DE FLAGS QUE ESTANDO TODO EN 0 NO SE SI HACE FALTA
	//this->position = Constants::BLOCK_HEADER_SIZE;
	this->occupiedRecords = 0;
}

void Block::printContent()
{
	this->seek(0);
	Record* record = new Record();
	while(this->getNextRecord(record) != NULL)
	{
		this->recordMethods->print(record->getBytes(), record->getSize());
		delete record;
		record = new Record();
	}

	delete record;
}

//SHOULD THIS METHOD BE ERASED?
/*UpdateResult Block::updateRecord(const char* key, Record* rec)
{
	Record* r = NULL;
	int startPosition = this->findRecord(key, &r);

	if (startPosition < 0)
	{
		if (r != NULL)
		{
			delete r;
		}
		return NOT_FOUND;
	}

	int sizeDifference = rec->getSize() - r->getSize();
	int occupiedSpace = this->getOccupiedSize();

	if (this->canInsertRecord(sizeDifference))
	{
		occupiedSpace += sizeDifference;
		// there is enough space to perform the update
		// in the current block
		int bufferSize = (this->maxSize - this->position) - sizeDifference;
		char* buffer = new char[bufferSize];
		memset(buffer, 0, bufferSize);

		// copy bytes that are after record
		memcpy(buffer, this->bytes + this->position, bufferSize);

		// update record
		int recordSize = rec->getSize();
		memcpy(this->bytes + startPosition, &recordSize, Constants::RECORD_HEADER_SIZE);
		memcpy(this->bytes + (startPosition + Constants::RECORD_HEADER_SIZE), rec->getBytes(), recordSize);

		// add
		memcpy(this->bytes + (startPosition + Constants::RECORD_HEADER_SIZE + recordSize), buffer, bufferSize);

		// update block size
		memcpy(this->bytes, &occupiedSpace, Constants::BLOCK_HEADER_SIZE);

		delete[] buffer;
	}
	else
	{
		// move to another block
		this->removeRecord(key);
		if (r != NULL)
		{
			delete r;
		}
		return INSUFFICIENT_SPACE;
	}

	if (r != NULL)
	{
		delete r;
	}
	return UPDATED;
}
*/
//SHOULD THIS METHOD BE ERASED?
/* void Block::forceInsert(Record *rec)
{
    int recSize = rec->getSize();
    int occupiedSpace = this->getOccupiedSize();

    if (occupiedSpace == 0)
    {
    	occupiedSpace += 4;
    }

    // add record size
    memcpy(this->bytes + occupiedSpace, &recSize, 4);
    occupiedSpace += 4;

    // add record data
    memcpy(this->bytes + occupiedSpace, rec->getBytes(), recSize);
    occupiedSpace += recSize;

    // update block size
    memcpy(this->bytes, &occupiedSpace, 4);
}
*/

bool Block::insertRecord(Record *rec)
	//	insert record in the current position.
{
	if (!this->canInsertRecord(rec->getSize()))
	{
		return false;
	}

    // add record data. this->bytes should always point to the first record.
    memcpy(this->bytes + this->currentRecord * recordSize, rec->getBytes(), rec->getSize() );
	this->occupiedRecords++;
    return true;
}

bool Block::canInsertRecord(int size)
{
	return this->recordSize >= size;
}

/*void Block::copyBlock(const Block & other)
{
    this->freeSpace = other.freeSpace;
    this->maxSize = other.maxSize;
    this->recordCount = other.recordCount;
    this->position = other.position;
    this->bytes = new char[this->maxSize];
    memset(this->bytes, 0, this->maxSize);
    memcpy(this->bytes, other.bytes, this->maxSize);
    this->recordMethods = other.recordMethods;
}*/

bool Block::removeRecord(const char* key)
	//remove the current record where stand
{
	if(! this->currentRecord->isFree()) return false;
	//SET 'ERASED' RECORD
	
	if(!this->isEmpty()) this->recordCount--;
	return true;
}

Block::~Block()
{
	delete[] this->bytes;
}
