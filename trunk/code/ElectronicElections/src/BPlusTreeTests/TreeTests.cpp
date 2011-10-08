/*
 * TreeTests.cpp
 *
 *  Created on: Sep 30, 2011
 *      Author: damian
 */

#include "TreeTests.h"
#include <iostream>
#include "../BPlusTree/Tree.h"
#include "../Entities/District.h"
#include "../Entities/DistrictMethods.h"
#include "../VariableBlocks/VariableRecord.h"


using namespace std;

TreeTests::TreeTests()
{
}

void TreeTests::printResult(string testName, bool result)
{
	cout << (testName.append(result ? ": Passed\n" : ": FAILED!!!\n")) << endl;
	cout << "========================================================";
	cout << endl;
}

void TreeTests::run()
{
	this->printResult("testInsertInEmptyTreeWorksCorrectly", testInsertInEmptyTreeWorksCorrectly());
	this->printResult("testInsertInRootWithOverflowCreatesTwoLeafs", testInsertInRootWithOverflowCreatesTwoLeafs());
	this->printResult("testInsertInLeafWithOverflowIsSplitByParent", testInsertInLeafWithOverflowIsSplitByParent());
	this->printResult("testInsertInInternalRootWithOverflowIsSplit", testInsertInInternalRootWithOverflowIsSplit());
	//this->printResult("testInsertInInternalNodeWithOverflowIsSplit", testInsertInInternalNodeWithOverflowIsSplit());
	this->printResult("testRemoveWithoutUnderflowWorksCorrectly", testRemoveWithoutUnderflowWorksCorrectly());
}

bool TreeTests::testInsertInEmptyTreeWorksCorrectly()
{
	DistrictMethods districtMethods;
	Tree tree("treeTests.dat", 512, &districtMethods, true);

	string districts[] = {"San Luis", "Santa Cruz", "Santa Fe", "Santiago del Estero",
			"Corrientes", "Tierra del Fuego", "Tucuman", "Entre Rios",
			"Chaco", "Chubut", "Cordoba",
			"Mendoza", "Misiones", "Neuquen", "Rio Negro", "Salta", "San Juan",
			"Buenos Aires", "Catamarca", "Formosa", "Jujuy", "La Pampa", "La Rioja",
			};

	for (int i = 0; i < 23; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());

		tree.insert(&keyRecord, &dataRecord);
	}

	tree.print();
	cout << endl;

	return true;
}

bool TreeTests::testInsertInRootWithOverflowCreatesTwoLeafs()
{
	DistrictMethods districtMethods;
	Tree tree("treeTests.dat", 128, &districtMethods, true);

	// approximately 223 chars
	string districts[] = {"San Luis", "Santa Cruz", "Santa Fe", "Santiago del Estero",
			"Corrientes", "Tierra del Fuego", "Tucuman", "Entre Rios",
			"Chaco", "Chubut", "Cordoba",
			"Mendoza", "Misiones", "Neuquen", "Rio Negro", "Salta", "San Juan",
			"Buenos Aires", "Catamarca", "Formosa", "Jujuy", "La Pampa", "La Rioja",
			};

	for (int i = 0; i < 7; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());

		tree.insert(&keyRecord, &dataRecord);
	}

	tree.print();
	cout << endl;

	return true;
}

bool TreeTests::testInsertInLeafWithOverflowIsSplitByParent()
{
	DistrictMethods districtMethods;
	Tree tree("treeTests.dat", 128, &districtMethods, true);

	// approximately 223 chars
	string districts[] = {"San Luis", "Santa Cruz", "Santa Fe", "Santiago del Estero",
			"Corrientes", "Tierra del Fuego", "Tucuman", "Entre Rios",
			"Chaco", "Chubut", "Cordoba",
			"Mendoza", "Misiones", "Neuquen", "Rio Negro", "Salta", "San Juan",
			"Buenos Aires", "Catamarca", "Formosa", "Jujuy", "La Pampa", "La Rioja",
			};

	for (int i = 0; i < 23; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());

		tree.insert(&keyRecord, &dataRecord);
	}

	tree.print();
	cout << endl;

	return true;
}

bool TreeTests::testInsertInInternalRootWithOverflowIsSplit()
{
	DistrictMethods districtMethods;
	Tree tree("treeTests.dat", 80, &districtMethods, true);

	// approximately 223 chars
	string districts[] = {"San Luis", "Santa Cruz",
				"Corrientes", "Entre Rios",
				"Chaco", "Chubut", "Cordoba",
				 "Santa Fe", "Santiago del Estero",
				"Mendoza", "Misiones", "Neuquen",
				"Tierra del Fuego", "Tucuman",
				"Rio Negro", "Salta", "San Juan",
				"Buenos Aires", "Catamarca", "Formosa",
				"Jujuy", "La Pampa", "La Rioja",
				};

	for (int i = 0; i < 13; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());

		tree.insert(&keyRecord, &dataRecord);
	}

	tree.print();
	cout << endl;

	return true;
}

bool TreeTests::testInsertInInternalNodeWithOverflowIsSplit()
{
	DistrictMethods districtMethods;
	Tree tree("treeTests.dat", 80, &districtMethods, true);

	// approximately 223 chars
	string districts[] = {"San Luis", "Santa Cruz",
				"Corrientes", "Entre Rios",
				"Chaco", "Chubut", "Cordoba",
				 "Santa Fe", "Santiago del Estero",
				"Mendoza", "Misiones", "Neuquen",
				"Tierra del Fuego", "Tucuman",
				"Rio Negro", "Salta", "San Juan",
				"Buenos Aires", "Catamarca", "Formosa",
				"Jujuy", "La Pampa", "La Rioja",
				};

	for (int i = 0; i < 13; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());

		tree.insert(&keyRecord, &dataRecord);
	}

	tree.print();
	cout << endl;

	return false;
}

bool TreeTests::testRemoveWithoutUnderflowWorksCorrectly()
{
	DistrictMethods districtMethods;
	Tree tree("treeTests.dat", 416, &districtMethods, true);

	string districts[] = {"San Luis", "Santa Cruz", "Santa Fe", "Santiago del Estero",
			"Corrientes", "Tierra del Fuego", "Tucuman", "Entre Rios",
			};

	for (int i = 0; i < 8; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());
		tree.insert(&keyRecord, &dataRecord);
	}
	tree.print();
	cout << endl << "Removing Tierra del Fuego..." << endl;
	District d("Tierra del Fuego");
	tree.remove(d.getKey());

	tree.print();
	cout << endl;

	return true;
}

TreeTests::~TreeTests()
{
}
