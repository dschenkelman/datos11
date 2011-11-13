//============================================================================
// Name        : ElectronicElections.cpp
// Author  : Datos11
// Version     :
// Copyright   :
// Description : Trying block file out
//============================================================================

#include "Menu/MainMenu.h"
#include "Voting/Menu.h"
#include <string>

using namespace std;

int main()
{
	option menuOption[2];
	menuOption[0].label = "Run Tests";
	menuOption[1].label = "Run Aplication";

	int menuAction = 0;
	Menu menu = Menu(menuOption, 2);

	do
	{
		menuAction = menu.ask();
	} while (menuAction != 1 && menuAction != 0);

	string fileName = "Files/config.txt";
	MainMenu mainMenu(fileName);

	if (menuAction == 0)
	{
		mainMenu.runTests();
	}

	else if (menuAction == 1)
	{
		mainMenu.runApplication();
	}

	return 0;
}
