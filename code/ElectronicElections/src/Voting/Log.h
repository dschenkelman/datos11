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
#include <time.h>
using namespace std;

class Log {
private:
	ofstream file;
public:
	Log();
	Log(string filename);
	virtual ~Log();
	Log& operator<<(string a) {
		time_t ltime; /* calendar time */
		ltime=time(NULL); /* get current cal time */
		char* timestamp = asctime( localtime(&ltime) );
		timestamp[string(timestamp).length()-1] = '\0';
		this->file << timestamp << " " << a << endl;
		//this->file.is_open() ? cout << "abierto" <<endl : cout << "cerrado" << endl;
		this->file.flush();
		return *this;
	}
	void write(string operation, bool result, bool use_stdout) {
		string res;
		result ? res = operation.append(" OK") : res = operation.append(" FAILED");
		this->operator <<(res);
		if (use_stdout) cout << res << endl;
	}
};

#endif /* LOG_H_ */
