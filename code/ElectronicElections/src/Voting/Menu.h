/*
 * Menu.h
 *
 *  Created on: 08/10/2011
 *      Author: alejandro
 */

#ifndef MENU_H_
#define MENU_H_

#include <iostream>
#include <string>

typedef struct option {
	std::string label;
} option;

class Menu {
private:
	option *options;
	int size;
public:
	Menu(option *opts, int size);
	virtual ~Menu();
	int ask();
	static std::string raw_input(std::string label);
};

#endif /* MENU_H_ */
