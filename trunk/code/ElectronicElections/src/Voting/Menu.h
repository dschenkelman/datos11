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
using namespace std;

typedef struct {
	string label;
} option;

class Menu {
private:
	option *options;
	int size;
public:
	Menu(option *opts, int size);
	virtual ~Menu();
	int ask();
};

#endif /* MENU_H_ */
