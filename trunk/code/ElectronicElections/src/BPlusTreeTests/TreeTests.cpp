/*
 * TreeTests.cpp
 *
 *  Created on: Sep 30, 2011
 *      Author: damian
 */

#include "TreeTests.h"
#include <iostream>
#include "../BPlusTree/Tree.h"
#include"../Entities/District.h"
#include "../Entities/DistrictMethods.h"
#include "../Entities/Count.h"
#include "../Entities/CountMethods.h"
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
	this->printResult("testInsertInInternalNodeWithOverflowIsSplit", testInsertInInternalNodeWithOverflowIsSplit());
	this->printResult("testInsertRecordWithDifferentKeyThanData", testInsertRecordWithDifferentKeyThanData());
	this->printResult("testRemoveWithoutUnderflowWorksCorrectly", testRemoveWithoutUnderflowWorksCorrectly());
	this->printResult("testRemoveInLeafWithUnderflowIsBalancedByParentWithRightBrother", testRemoveInLeafWithUnderflowIsBalancedByParentWithRightBrother());
	this->printResult("testRemoveInRightMostLeafWithUnderflowIsBalancedByParentWithLeftBrother", testRemoveInRightMostLeafWithUnderflowIsBalancedByParentWithLeftBrother());
	this->printResult("testRemoveInLeafWithUnderflowAndRightBrotherInAnotherParentBalancesCorrectly", testRemoveInLeafWithUnderflowAndRightBrotherInAnotherParentBalancesCorrectly());
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

	int i;
	for (i = 0; i < 23; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());

		tree.insert(&keyRecord, &dataRecord);
	}

	cout << "District Count" << i << endl;
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

	int i;
	for (i = 0; i < 7; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());

		tree.insert(&keyRecord, &dataRecord);
	}

	cout << "District Count" << i << endl;
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

	int i;
	for (i = 0; i < 23; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());

		tree.insert(&keyRecord, &dataRecord);
	}

	cout << "District Count" << i << endl;
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

	int i;
	for (i = 0; i < 13; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());

		tree.insert(&keyRecord, &dataRecord);
	}

	cout << "District Count" << i << endl;
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

	int i;
	for (i = 0; i < 23; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());

		tree.insert(&keyRecord, &dataRecord);
	}

	cout << "District Count" << i << endl;

	tree.print();
	cout << endl;

	return true;
}

bool TreeTests::testInsertRecordWithDifferentKeyThanData()
{
	Count c1(9, 10, 2010, "Presidente", "Lista1", "Cordoba", 500);
	Count c2(20, 5, 1990, "Presidente", "Lista2", "Cordoba", 500);
	Count c3(30, 12, 1980, "Presidente", "Lista4", "Cordoba", 500);
	Count c4(5, 8, 1995, "Presidente", "Lista3", "Cordoba", 500);
	Count c5(7, 7, 2010, "Presidente", "Lista4", "Cordoba", 500);
	Count c6(1, 3, 1970, "Presidente", "Lista5", "Cordoba", 500);

	Count counts[] = {c1, c2, c3, c4, c5, c6};
	CountMethods countMethods;
	Tree tree("treeTests.dat", 128, &countMethods, true);

	int i;
	for (i = 0; i < 6; i++)
	{
		Count c = counts[i];
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(c.getBytes(), c.getSize());
		keyRecord.setBytes(c.getKey(), c.getKeySize());

		tree.insert(&keyRecord, &dataRecord);
	}

	cout << "Counts Count" << i << endl;
	tree.print();
	cout << endl;

	return true;
}

bool TreeTests::testRemoveWithoutUnderflowWorksCorrectly()
{
	DistrictMethods districtMethods;
	Tree tree("treeTests.dat", 416, &districtMethods, true);

	string districts[] = {"San Luis", "Santa Cruz", "Santa Fe", "Santiago del Estero",
			"Corrientes", "Tierra del Fuego", "Tucuman", "Entre Rios",
			};

	int i;
	for (i = 0; i < 8; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());
		tree.insert(&keyRecord, &dataRecord);
	}

	cout << "District Count" << i << endl;
	tree.print();
	cout << endl << "Removing Tierra del Fuego..." << endl;
	District d("Tierra del Fuego");
	tree.remove(d.getKey());

	cout << "District Count" << i - 1 << endl;
	tree.print();
	cout << endl;

	return true;
}

bool TreeTests::testRemoveInLeafWithUnderflowIsBalancedByParentWithRightBrother()
{
	DistrictMethods districtMethods;
	Tree tree("treeTests.dat", 128, &districtMethods, true);

	// approximately 78 chars
	string districts[] = {"San Luis", "Santa Cruz",
					"Corrientes", "Entre Rios", "Chaco", "Chubut",
					"Cordoba","Tierra del Fuego", "Santa Fe",};

	int i;
	for (i = 0; i < 9; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());

		tree.insert(&keyRecord, &dataRecord);
	}

	cout << "District Count" << i << endl;
	tree.print();

	cout << endl << "Remove in the left child => it gets underflow" << endl;
	District d("Entre Rios");
	tree.remove(d.getKey());

	cout << endl << "The tree is balanced" << endl;
	cout << "District Count" << i - 1 << endl;
	tree.print();

	cout << endl;
	return true;
}

bool TreeTests::testRemoveInRightMostLeafWithUnderflowIsBalancedByParentWithLeftBrother()
{
	DistrictMethods districtMethods;
	Tree tree("treeTests.dat", 128, &districtMethods, true);

	// approximately 78 chars
	string districts[] = {"San Luis", "Santa Cruz",
					"Corrientes", "Entre Rios", "Chaco", "Chubut",
					"Cordoba","Tierra del Fuego", "Santa Fe","Buenos Aires"};

	int i;
	for (i = 0; i < 10; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());

		tree.insert(&keyRecord, &dataRecord);
	}

	cout << "District Count" << i + 1 <<endl;
	tree.print();

	cout << endl << "Remove in the left child => it gets underflow" << endl;
	District d("Tierra del Fuego");
	tree.remove(d.getKey());

	cout << endl << "The tree is balanced" << endl;
	cout << "District Count" << i - 1 << endl;
	tree.print();

	cout << endl;
	return true;
}

bool TreeTests::testRemoveInLeafWithUnderflowAndRightBrotherInAnotherParentBalancesCorrectly()
{
	DistrictMethods districtMethods;
	Tree tree("treeTests.dat", 100, &districtMethods, true);

	// approximately 223 chars
	string districts[] = {"San Luis", "Santa Cruz",
				"Corrientes", "Entre Rios",
				"Chaco", "Chubut", "Cordoba",
				 "Santa Fe", "Santiago del Estero",
				"Mendoza", "Misiones", "Neuquen",
				"Tierra del Fuego", "Tucuman",
				 "Salta", "San Juan",
				"Buenos Aires", "Catamarca", "Formosa",
				"Jujuy", "La Pampa", "La Rioja","Rio Negro",
				};

	int i;
	for (i = 0; i < 18; i++)
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

bool TreeTests::testRemoveInLeafWithUnderflowIsMergedByParent()
{
	return false;
}

TreeTests::~TreeTests()
{
	cout << endl;
}
