/*
 * Voting.h
 *
 *  Created on: 08/10/2011
 *      Author: alejandro
 */

#ifndef VOTING_H_
#define VOTING_H_

#include <string>
using namespace std;

class Voting {
public:
	Voting();
	virtual ~Voting();
	bool voted(string dni);
};

#endif /* VOTING_H_ */
