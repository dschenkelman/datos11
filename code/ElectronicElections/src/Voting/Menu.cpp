/*
 * Menu.cpp
 *
 *  Created on: 08/10/2011
 *      Author: alejandro
 */

#include <string.h>
#include <iostream>
using namespace std;

typedef struct {
	string label;
} option ;

int ask_user(option *opt, int size) {
	cout << "Seleccione:" << endl;
	option *cur_opt = opt;
	for (int i=0;i<size;i++) {
		cout << i << ". " << cur_opt[i].label << endl;
	}
	int ret_value;
	cin >> ret_value;
	return ret_value;
}

int menu_usage() {
	option *opts = new option[3];
	opts[0].label = "opcion uno";
	opts[1].label = "dos";
	opts[2].label = "tres";
	cout << ask_user(opts,3) << endl ;
	return 0;
}
