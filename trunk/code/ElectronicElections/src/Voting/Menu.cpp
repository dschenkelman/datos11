/*
 * Menu.cpp
 *
 *  Created on: 08/10/2011
 *      Author: alejandro
 */

#include "Menu.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

Menu::Menu(option *opts, int size) {
	this->options = opts;
	this->size = size;
}

Menu::~Menu() {
}

int Menu::ask() {
//	cout << "\033[2J\033[1;1H";
	cout << endl << "****************************************" << endl << "Seleccione:" << endl;
	option *cur_opt = this->options;
	for (int i = 0; i < this->size; i++) {
		cout << i << ". " << cur_opt[i].label << endl;
	}
	int ret_value;
	string input = "";
	getline(cin, input);
	stringstream myStream(input);
	if (!(myStream >> ret_value))
	{
		return -1;
	}
	return ret_value;
}

string Menu::raw_input(string label) {
	cout << label << "? ";

	char ret_value[512];
	cin.getline (ret_value,512,'\n');
	string ret (ret_value);
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
