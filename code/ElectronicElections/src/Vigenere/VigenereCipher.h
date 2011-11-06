/*
 * VigenereCipher.h
 *
 *  Created on: Nov 6, 2011
 *      Author: damian
 */

#ifndef VIGENERECIPHER_H_
#define VIGENERECIPHER_H_

#include <string>

class VigenereCipher {
public:
	VigenereCipher();
	std::string encript(std::string& message, std::string& key);
	std::string decript(std::string& message, std::string& key);
	virtual ~VigenereCipher();
};

#endif /* VIGENERECIPHER_H_ */
