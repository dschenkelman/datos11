/*
 * Charge.h
 *
 *  Created on: 06/10/2011
 *      Author: lejosdelcielo
 */

#include <string>
#include <vector>

#ifndef CHARGE_H_
#define CHARGE_H_

class Charge {
private:
	std::string charge;
	std::vector<std::string> chargeList;
	char* bytes;
	char* key;
public:
	Charge(std::string charge, std::vector<std::string> chargeList);
	Charge(Charge& other);
	Charge& operator=(Charge& other);
	char* getBytes();
	int getSize();
	char* getKey();
	int getKeySize();
	void setBytes(char* bytes);
	virtual ~Charge();
	//getters
	std::string getCharge();
	std::vector<std::string> getChargeList();
};

#endif /* CHARGE_H_ */
