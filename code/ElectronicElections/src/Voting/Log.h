/*
 * Log.h
 *
 *  Created on: 08/10/2011
 *      Author: alejandro
 */

#ifndef LOG_H_
#define LOG_H_

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Log {
private:
	ofstream file;
public:
	Log();
	Log(string filename);
	Log* operator <<(const string b);
	virtual ~Log();
};

#endif /* LOG_H_ */
