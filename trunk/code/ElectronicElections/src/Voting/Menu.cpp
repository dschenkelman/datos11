/*
 * Menu.cpp
 *
 *  Created on: 08/10/2011
 *      Author: alejandro
 */

#include "Menu.h"

Menu::Menu(option *opts, int size) {
	this->options = opts;
	this->size = size;
}

Menu::~Menu() {
}

int Menu::ask() {
	cout << "Seleccione:" << endl;
	option *cur_opt = this->options;
	for (int i = 0; i < this->size; i++) {
		cout << i << ". " << cur_opt[i].label << endl;
	}
	int ret_value;
	cin >> ret_value;
	return ret_value;
}

string Menu::raw_input(string label) {
	cout << label << "? ";
	string ret;
	cin >> ret;
	return ret;
}

//int menu_usage() {
//	option *opts = new option[3];
//	opts[0].label = "opcion uno";
//	opts[1].label = "dos";
//	opts[2].label = "tres";
//	cout << ask_user(opts, 3) << endl;
//	return 0;
//}
