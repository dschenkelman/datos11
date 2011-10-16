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
#include "../Entities/Administrator.h"
#include "../Entities/AdministratorMethods.h"
#include "../VariableBlocks/VariableRecord.h"
#include <string.h>

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
	this->printResult("testRemoveInLeafWithUnderflowIsMergedByParent", testRemoveInLeafWithUnderflowIsMergedByParent());
	this->printResult("testRemoveInLeafCausingUnderflowInParentIsCorrectlyBalancedWhenNonRightMost", testRemoveInLeafCausingUnderflowInParentIsCorrectlyBalancedWhenNonRightMost());
	this->printResult("testRemoveInLeafCausingUnderflowInParentIsCorrectlyBalancedWhenRightMost", testRemoveInLeafCausingUnderflowInParentIsCorrectlyBalancedWhenRightMost());
	this->printResult("testRemoveCausesUnderflowInRootIsMergedIntoLeaf", testRemoveCausesUnderflowInRootIsMergedIntoLeaf());
	this->printResult("testRemoveShouldMergeInternalNodes", testRemoveShouldMergeInternalNodesWhenNotRightMost());
	this->printResult("testRemoveShouldMergeInternalNodesWhenRightMost", testRemoveShouldMergeInternalNodesWhenRightMost());
	this->printResult("testRemoveShouldStoreAvailableFreeBlocks", testRemoveShouldStoreAvailableFreeBlocks());
	this->printResult("testShouldUpdateBlocksInLeaf", testUpdateBlocksInLeaf());
	this->printResult("testUpdateBlockInLeafWithOverflowCreatesTwoLeafs", testUpdateBlockInLeafWithOverflowCreatesTwoLeafs());
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
	Tree tree("treeTests.dat", 84, &districtMethods, true);

	// approximately 223 chars
	string districts[] = {
			"Buenos Aires", "Catamarca",
			"Chaco", "Chubut", "Cordoba",
			"Corrientes", "Entre Rios", "Formosa",
			"Jujuy", "La Pampa", "La Rioja",
			"Mendoza", "Misiones", "Neuquen",
				"Rio Negro", "Salta", "San Juan",
				"Santa Fe", "Santiago del Estero",
				"San Luis", "Santa Cruz",
				"Tierra del Fuego", "Tucuman",
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
	District d("Corrientes");
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
					"Corrientes", "Entre Rios", "Chaco", "Chubut", "Artigas",
					"Cordoba","Tierra del Fuego", "Santa Fe","Buenos Aires",
					"Santiago del Estero", "Maldonado", "Mendoza"};

	int i;
	for (i = 0; i < 14; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());

		tree.insert(&keyRecord, &dataRecord);
	}

	cout << "District Count" << i <<endl;
	tree.print();

	cout << endl << "Remove in the right most child 'Tierra del Fuego'" << endl;
	District d("Tierra del Fuego");
	tree.remove(d.getKey());

	cout << endl << "Remove in the right most child 'Santa Fe'. 'San Luis' is balanced" << endl;
	District d1("Santa Fe");
	tree.remove(d1.getKey());

	cout << endl << "The tree is balanced" << endl;
	cout << "District Count" << i - 2 << endl;
	tree.print();

	cout << endl;
	return true;
}

bool TreeTests::testRemoveInLeafWithUnderflowIsMergedByParent()
{
	DistrictMethods districtMethods;
	District *distric;
	Tree tree("treeTests.dat", 96, &districtMethods, true);

	// approximately n chars
		string districts[] = {"San Luis", "Santa Cruz",
						"Corrientes", "Entre Rios",
						"Chaco", "Chubut", "Cordoba",
						 "Santa Fe", //"Santiago del Estero",
						"Mendoza", "Misiones"//, "Neuquen"
						};

	for (int i = 0; i < 10; i++)
	{
		this->insertInTree(&tree,districts[i]);
	}

	tree.print();

	cout << endl << "Remove in the right child 'Santa Fe' => underflow => merge" << endl;
	distric = new District("Santa Fe");
	tree.remove(distric->getKey());
	tree.print();
	delete distric;

	cout << endl << "Reinsert 'Santa Fe' => overflow => split" << endl;
	this->insertInTree(&tree,"Santa Fe");
	tree.print();

	cout << endl << "Remove in the middle child 'Entre Rios' => underflow => merge" << endl;
	distric = new District("Entre Rios");
	tree.remove(distric->getKey());
	tree.print();
	delete distric;

	cout << endl << "Reinsert 'Entre Rios' => overflow => split" << endl;
	this->insertInTree(&tree,"Entre Rios");
	tree.print();

	cout << endl << "Remove in the first child 'Cordoba', then 'Corrientes' => underflow => merge" << endl;
	distric = new District("Cordoba");
	tree.remove(distric->getKey());
	delete distric;
	distric = new District("Corrientes");
	tree.remove(distric->getKey());
	delete distric;
	tree.print();

	cout << endl;

	return true;
}


void TreeTests::insertInTree(Tree* tree, string key)
{
	District d(key);
	VariableRecord dataRecord;
	VariableRecord keyRecord;
	dataRecord.setBytes(d.getBytes(), d.getSize());
	keyRecord.setBytes(d.getKey(), d.getKeySize());
	tree->insert(&keyRecord, &dataRecord);
}


bool TreeTests::testRemoveInLeafCausingUnderflowInParentIsCorrectlyBalancedWhenNonRightMost()
{
	DistrictMethods districtMethods;
	Tree tree("treeTests.dat", 90, &districtMethods, true);

	string districts[] = {	"Corrientes", "Entre Rios",
				"Chaco", "Chubut", "Cordoba",
				 "Santa Fe", "Santiago del Estero",
				"Mendoza", "Misiones", "Neuquen",
				"Tierra del Fuego", "Tucuman",
				 "Salta", "San Juan",
				 "San Luis", "Santa Cruz",
				"Buenos Aires", "Catamarca", "Formosa",
				"Jujuy", "La Pampa", "La Rioja","Rio Negro",
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

	cout << "District Count: " << i << endl;
	tree.print();
	cout << endl;


	District d("Corrientes");
	tree.remove(d.getKey());

	District d1("Entre Rios");
	tree.remove(d1.getKey());

	cout << "Removing Corrientes and Entre Rios. First node in level 1 goes into"
			" underflow and is balanced with brother." << endl;
	cout << "District Count: " << i - 2 << endl;
	tree.print();
	cout << endl;

	return true;
}

bool TreeTests::testRemoveInLeafCausingUnderflowInParentIsCorrectlyBalancedWhenRightMost()
{
	DistrictMethods districtMethods;
	Tree tree("treeTests.dat", 90, &districtMethods, true);

	string districts[] = {
				 "Santa Fe", "Santiago del Estero",
				"Mendoza", "Misiones", "Neuquen",
				"Tierra del Fuego", "Tucuman",
				 "Salta", "San Juan",
				 "Jujuy", "La Pampa", "La Rioja","Rio Negro",
				 "San Luis", "Santa Cruz",
				 "Corrientes", "Entre Rios",
				"Chaco", "Chubut", "Cordoba",
				"Buenos Aires", "Catamarca", "Formosa",
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

	cout << "District Count: " << i << endl;
	tree.print();
	cout << endl;


	District d("Santiago del Estero");
	tree.remove(d.getKey());

	cout << "Removing Santiago del Estero. Last node in level 1 goes into"
			" underflow and is balanced with brother." << endl;
	cout << "District Count: " << i - 1 << endl;
	tree.print();
	cout << endl;

	return true;
}


bool TreeTests::testRemoveShouldMergeInternalNodesWhenNotRightMost()
{
	DistrictMethods districtMethods;
	Tree tree("treeTests.dat", 90, &districtMethods, true);

	string districts[] = {"San Luis", "Santa Cruz",
					"Tierra del Fuego", "Tucuman",
					"Rio Negro", "Salta", "San Juan",
					 "Santa Fe", "Santiago del Estero",
					 "Mendoza", "Misiones", "Neuquen",
					"Jujuy", "La Pampa", "La Rioja",
					"Chaco", "Chubut!!Chubut!!", "Cordoba",
					"Corrientes", "Entre Rios",
					"Buenos Aires", "Catamarca", "Formosa",
					"Dakhala", "El Cairo islamico", "Masai Mara",
					"Glaciar Perito Moreno", "Kilimanjaro", "Marrakech",
					"Iguazu", "Xauen", "Seychelles Rocks",
					};

	int i;
	for (i = 0; i < 24; i++)
	{
		insertInTree(&tree,districts[i]);
	}

	cout << "District Count" << i << endl;
	tree.print();

	District err("Entre Rios");
	tree.remove(err.getKey());
	cout << endl << "REMOVE Entre Rios";
	cout<<endl;tree.print();cout<<endl;

	District se("Santiago del Estero");
	tree.remove(se.getKey());
	cout << endl << "REMOVE Santiago del Estero";
	cout<<endl;tree.print();cout<<endl;

	District me("Mendoza");
	tree.remove(me.getKey());
	cout << endl << "REMOVE Mendoza";
	cout<<endl;tree.print();cout<<endl;

	District rn("Rio Negro");
	tree.remove(rn.getKey());
	cout << endl << "REMOVE Rio Negro";
	cout<<endl;tree.print();cout<<endl;

	District mi("Misiones");
	tree.remove(mi.getKey());
	cout << endl << "REMOVE Misiones";
	cout<<endl;tree.print();cout<<endl;

	District cord("Cordoba");
	tree.remove(cord.getKey());
	cout << endl << "REMOVE Cordoba";
	cout<<endl;tree.print();cout<<endl;

	District corr("Corrientes");
	tree.remove(corr.getKey());
	cout << endl << "REMOVE Corrientes";
	cout<<endl;tree.print();cout<<endl;

	return true;
}


bool TreeTests::testRemoveCausesUnderflowInRootIsMergedIntoLeaf()
{
	DistrictMethods districtMethods;
	string fileName = "treeTests.dat";
	Tree tree(fileName, 70, &districtMethods, true);

	string districts[] = {
				"Acre", "Alagoas", "Amapa", "Amazonas", "Bahia", "Ceara",
				};

	int i;
	for (i = 0; i < 6; i++)
	{
		District d(districts[i]);
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(d.getBytes(), d.getSize());
		keyRecord.setBytes(d.getKey(), d.getKeySize());

		tree.insert(&keyRecord, &dataRecord);
	}

	cout << "District Count: " << i << endl;
	tree.print();
	cout << endl;

	District d("Alagoas");
	tree.remove(d.getKey());
	cout << "Removed Alagoas" << endl;
	tree.print();
	cout << endl;

	District d1("Amazonas");
	tree.remove(d1.getKey());
	cout << "Removed Amazonas, internal root goes into underflow and is transformed into leaf." << endl;
	tree.print();
	cout << endl;

	return false;
}

bool TreeTests::testRemoveShouldMergeInternalNodesWhenRightMost()
{
	DistrictMethods districtMethods;
	Tree tree("treeTests.dat", 90, &districtMethods, true);

	string districts[] = {
				 "Santa Fe", "Santiago del Estero",
				"Mendoza", "Misiones", "Neuquen",
				"Tierra del Fuego", "Tucuman",
				 "Salta", "San Juan",
				 "Jujuy", "La Pampa", "La Rioja","Rio Negro",
				 "San Luis", "Santa Cruz",
				 "Corrientes", "Entre Rios",
				"Chaco", "Chubut", "Cordoba",
				"Buenos Aires", "Catamarca", "Formosa",
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

	cout << "District Count: " << i << endl;
	tree.print();
	cout << endl;

	District d("Santiago del Estero");
	tree.remove(d.getKey());

	cout << "District Count: " << i - 1 << endl;
	tree.print();
	cout << endl;

	return true;

}

bool TreeTests::testGetInATree()
{
	DistrictMethods districtMethods;

	Tree tree("treeTests.dat", 90, &districtMethods, true);

	string districts[] = {
				 "Santa Fe", "Santiago del Estero",
				"Mendoza", "Misiones", "Neuquen",
				"Tierra del Fuego", "Tucuman",
				 "Salta", "San Juan",
				 "Jujuy", "La Pampa", "La Rioja","Rio Negro",
				 "San Luis", "Santa Cruz",
				 "Corrientes", "Entre Rios",
				"Chaco", "Chubut", "Cordoba",
				"Buenos Aires", "Catamarca", "Formosa",
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

	cout << "District Count: " << i << endl;
	tree.print();
	cout << endl;

	/*District d("Chubut");
	VariableRecord r;
	tree.get(d.getKey(),&r);

	cout << "Search for Chubut..." << endl;
	cout << "Key obtained: " << endl;
	districtMethods.print(r.getBytes(),r.getSize());
	cout << endl;*/

	VariableRecord r;
	tree.getNext(&r);
	districtMethods.print(r.getBytes(),r.getSize());
	/*District e("La Pampa");
	tree.get(e.getKey(),&r);
	cout << "Search for La Pampa..." << endl;
	cout << "Key obtained: " << endl;
	districtMethods.print(r.getBytes(),r.getSize());
	cout << endl;

	District f("Cecilia");
	tree.get(f.getKey(),&r);
	cout << "Search for Cecilia... (it must return the next record found)" << endl;
	cout << "Key obtained: " << endl;
	districtMethods.print(r.getBytes(),r.getSize());
	cout << endl;

	District g("Rio Ceballos");
	tree.get(g.getKey(),&r);
	cout << "Search for Rio Ceballos... (it must return the next record found)" << endl;
	cout << "Key obtained: " << endl;
	districtMethods.print(r.getBytes(),r.getSize());
	cout << endl;*/


	return false;
}

bool TreeTests::testRemoveShouldStoreAvailableFreeBlocks()
{
	DistrictMethods districtMethods;
	Tree tree("treeTests.dat", 90, &districtMethods, true);

	string districts[] = {"San Luis", "Santa Cruz",
					"Tierra del Fuego", "Tucuman",
					"Rio Negro", "Salta", "San Juan",
					 "Santa Fe", "Santiago del Estero",
					 "Mendoza", "Misiones", "Neuquen",
					"Jujuy", "La Pampa", "La Rioja",
					"Chaco", "Chubut!!Chubut!!", "Cordoba",
					"Corrientes", "Entre Rios",
					"Buenos Aires", "Catamarca", "Formosa",
					"Dakhala", "El Cairo islamico", "Masai Mara",
					"Glaciar Perito Moreno", "Kilimanjaro", "Marrakech",
					"Iguazu", "Xauen", "Seychelles Rocks",
					};

	int i;
	for (i = 0; i < 24; i++)
	{
		insertInTree(&tree,districts[i]);
	}

	cout << "District Count" << i << endl;
	tree.print();

	District err("Entre Rios");
	tree.remove(err.getKey());
	cout << endl << "REMOVE Entre Rios";
	cout<<endl;tree.print();cout<<endl;

	District se("Santiago del Estero");
	tree.remove(se.getKey());
	cout << endl << "REMOVE Santiago del Estero";
	cout<<endl;tree.print();cout<<endl;

	District me("Mendoza");
	tree.remove(me.getKey());
	cout << endl << "REMOVE Mendoza";
	cout<<endl;tree.print();cout<<endl;

	District rn("Rio Negro");
	tree.remove(rn.getKey());
	cout << endl << "REMOVE Rio Negro";
	cout<<endl;tree.print();cout<<endl;

	District mi("Misiones");
	tree.remove(mi.getKey());
	cout << endl << "REMOVE Misiones";
	cout<<endl;tree.print();cout<<endl;

	District cord("Cordoba");
	tree.remove(cord.getKey());
	cout << endl << "REMOVE Cordoba";
	cout<<endl;tree.print();cout<<endl;

	District corr("Corrientes");
	tree.remove(corr.getKey());
	cout << endl << "REMOVE Corrientes";
	cout<<endl;tree.print();cout<<endl;

	insertInTree(&tree,"Corrientes");
	insertInTree(&tree,"Cordoba");
	insertInTree(&tree,"Misiones");
	insertInTree(&tree,"Rio Negro");
	insertInTree(&tree,"Mendoza");
	insertInTree(&tree,"Santiago del Estero");
	insertInTree(&tree,"Entre Rios");

	cout << endl << "Added all back" << endl;
	tree.print();cout<<endl;

	return true;
}

bool TreeTests::testUpdateBlocksInLeaf()
{
		Administrator a1("A1", "P1");
		Administrator a2("A2", "P2");
		Administrator a3("A3", "P3");
		Administrator a4("A4", "P4");
		Administrator a5("A5", "P5");
		Administrator a6("A6", "P6");

		Administrator admins[] = {a1, a2, a3, a4, a5, a6};
		AdministratorMethods adminMethods;
		Tree tree("treeTests.dat", 128, &adminMethods, true);

		int i;
		for (i = 0; i < 6; i++)
		{
			Administrator a = admins[i];
			VariableRecord dataRecord;
			VariableRecord keyRecord;
			dataRecord.setBytes(a.getBytes(), a.getSize());
			keyRecord.setBytes(a.getKey(), a.getKeySize());

			tree.insert(&keyRecord, &dataRecord);
		}

		cout << "Admins Count" << i << endl;
		tree.print();
		cout << endl;

		string pass = "Pass";
		string pass1 = "Pas5";

		a3.setPassword(pass);

		a4.setPassword(pass);

		a5.setPassword(pass1);

		VariableRecord r3(a3.getBytes(), a3.getSize());
		VariableRecord r4(a4.getBytes(), a4.getSize());
		VariableRecord r5(a5.getBytes(), a5.getSize());
		tree.update(a3.getKey(), &r3);
		tree.update(a4.getKey(), &r4);
		tree.update(a5.getKey(), &r5);

		tree.print();
		cout << endl;

		return true;
}

bool TreeTests::testUpdateBlockInLeafWithOverflowCreatesTwoLeafs()
{
	Administrator a1("Adm1", "P1");
	Administrator a2("Adm2", "P2");
	Administrator a3("Adm3", "P3");

	Administrator admins[] = {a1, a2, a3};
	AdministratorMethods adminMethods;
	Tree tree("treeTests.dat", 64, &adminMethods, true);

	int i;
	for (i = 0; i < 3; i++)
	{
		Administrator a = admins[i];
		VariableRecord dataRecord;
		VariableRecord keyRecord;
		dataRecord.setBytes(a.getBytes(), a.getSize());
		keyRecord.setBytes(a.getKey(), a.getKeySize());

		tree.insert(&keyRecord, &dataRecord);
	}

	cout << "Admins Count " << i << endl;
	tree.print();
	cout << endl;

	string pass1 = "Pas1";
	string pass2 = "Pas2";

	a1.setPassword(pass1);

	a2.setPassword(pass2);

	VariableRecord r1(a1.getBytes(), a1.getSize());
	VariableRecord r2(a2.getBytes(), a2.getSize());
	tree.update(a1.getKey(), &r1);
	tree.update(a2.getKey(), &r2);

	cout << "After Update" << endl;
	tree.print();
	cout << endl;

	return false;
}

TreeTests::~TreeTests()
{
	cout << endl;
}
