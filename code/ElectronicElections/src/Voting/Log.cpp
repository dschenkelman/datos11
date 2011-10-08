/*
 * Log.cpp
 *
 *  Created on: 08/10/2011
 *      Author: alejandro
 */

#include "Log.h"

Log::Log(string filename) {
	this->file.open(filename.c_str(), ios::out | ios::app);
}

Log::Log() {
	Log("log.txt");
}

Log::~Log() {
	this->file.close();
}

Log* Log::operator <<(const string b) {
    time_t ltime; /* calendar time */
    ltime=time(NULL); /* get current cal time */
    this->file << asctime( localtime(&ltime) ) << b;
	return this;
}
