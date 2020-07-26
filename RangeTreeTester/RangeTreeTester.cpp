// RangeTreeTester.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "..\RangeTreeDataStructure\RangeTree.h"
#include "RangeTreeTesterGlobals.h"
#include <map>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


using namespace rangetree;


void addNodes3LevelTestCase(RangeTree<u32, std::string>* rangeNode)
{
	std::string str= "HelloWorld";
	rangeNode->insert(50,70,str);
	str.clear();
	str = "Hi";
	rangeNode->insert(25,45,str);
	str.clear();
	str = "Hogo";
	rangeNode->insert(75,80,str);
	str.clear();
	str = "Pogo";
	rangeNode->insert(10,12,str);
	str.clear();
	str = "solo";
	rangeNode->insert(30,31,str);
	str.clear();
	str = "gigi";
	rangeNode->insert(60,75,str);
	str.clear();
	str = "riri";
	rangeNode->insert(90,100,str);
	rangeNode->insert(55,75,str);
	rangeNode->insert(65,70,str);
	rangeNode->insert(80,88,str);
	rangeNode->insert(95,99,str);

	rangeNode->insert(8,20,str);
	rangeNode->insert(15,34,str);
	rangeNode->insert(27,31,str);
	rangeNode->insert(35,45,str);
	rangeNode->insert(17,27,str);

	rangeNode->deleteNode(90,8);
	rangeNode->deleteNode(55,8);
	rangeNode->deleteNode(65,8);
	rangeNode->deleteNode(80,8);
	rangeNode->deleteNode(95,8);

	rangeNode->deleteNode(8,8);
	rangeNode->deleteNode(15,8);
	rangeNode->deleteNode(-10,8);
	rangeNode->deleteNode(35,8);
	rangeNode->deleteNode(1000,8);
	rangeNode->deleteNode(15, 8);
}


void singleAddLeft(RangeTree<u32, std::string>* rangeNode)
{
	std::string str= "HelloWorld";
	rangeNode->insert(90,8,str);
	rangeNode->insert(70,8,str);
	
rangeNode->insert(50,8,str);
rangeNode->insert(20,8,str);
rangeNode->insert(10,8,str);
rangeNode->insert(55,8,str);
	rangeNode->deleteNode(55, 8);
}
void deleteNodesTestCase(RangeTree<u32, std::string>* rangeNode)
{
	rangeNode->deleteNode(90, 8);
	rangeNode->deleteNode(60, 8);
	rangeNode->deleteNode(65, 8);
	rangeNode->deleteNode(35, 8);
	rangeNode->deleteNode(50, 8);
	rangeNode->deleteNode(55, 8);
	rangeNode->deleteNode(8, 8);
	//std::string val = *((rangeNode->p_head->p_right->p_left)->m_pValue);
}

void addNodes1level(RangeTree<u32, std::string>* rangeNode)
{
	std::string str= "HelloWorld";
	rangeNode->insert(50,8,str);
	str.clear();
	str = "Hi";
	rangeNode->insert(25,8,str);
	str.clear();
	str = "Hogo";
	rangeNode->insert(75,8,str);
	rangeNode->deleteNode(50, 8);
}

void addNodes2Level(RangeTree<u32, std::string>* rangeNode)
{
	std::string str= "HelloWorld";
	rangeNode->insert(50,8,str);
	str.clear();
	str = "Hi";
	rangeNode->insert(25,8,str);
	str.clear();
	str = "Hogo";
	rangeNode->insert(75,8,str);
	str.clear();
	str = "Pogo";
	rangeNode->insert(10,8,str);
	str.clear();
	str = "solo";
	rangeNode->insert(30,8,str);
	str.clear();
	str = "gigi";
	rangeNode->insert(60,8,str);
	str.clear();
	str = "riri";
	rangeNode->insert(90,8,str);
	rangeNode->deleteNode(75, 8);
}

void singleAddRight(RangeTree<u32, std::string>* rangeNode)
{
	std::string str= "HelloWorld";
	rangeNode->insert(10,8,str);
	rangeNode->insert(30,8,str);
	
	rangeNode->insert(50,8,str);
	rangeNode->insert(70,8,str);
	rangeNode->insert(45,8,str);
	rangeNode->insert(1,8,str);

	rangeNode->deleteNode(45, 8);
	rangeNode->deleteNode(10, 8);
	rangeNode->deleteNode(1, 8);
	rangeNode->deleteNode(30, 8);
	rangeNode->deleteNode(50, 8);
	rangeNode->deleteNode(70, 8);
}

void searchTestCases(RangeTree<u32, std::string>* rangeNode)
{
	std::list<std::string> liststr;
	std::string val_str;
	val_str.clear();
	val_str = "riri";
	
	rangeNode->insert(50,55, val_str);
	rangeNode->insert(5,15,val_str);
	rangeNode->insert(5,15,val_str);
	rangeNode->findAll(20, liststr);
	bool isit = rangeNode->find(20);
}

void testMemoryLeak()
{
	RangeTree<u32, std::string>* rangeNode = new RangeTree<u32, std::string>(10);
	std::string str= "HelloWorld";
	rangeNode->insert(50,70,str);

	addNodes1level(rangeNode);
    addNodes2Level(rangeNode);
	addNodes3LevelTestCase(rangeNode);
	
	singleAddLeft(rangeNode);
	singleAddRight(rangeNode);
	deleteNodesTestCase(rangeNode);
	searchTestCases(rangeNode);

	rangeNode->clear();
	delete rangeNode;
	
}



class TestClass
{
public:
	char m;
	TestClass(){};
	TestClass(char m){this->m = m;};
	TestClass( TestClass& other)
	{
		this->m = other.m;
	}
	bool operator==(const TestClass& other) const
	{
		return this->m == other.m;
	}

	bool operator< (const TestClass& other) const
	{
		return this->m < other.m;
	}

	bool operator>=(const TestClass& other) const
	{
		return this->m >= other.m;
	}

	bool operator<=(const TestClass& other) const
	{
		return this->m <= other.m;
	}

	bool operator>(const TestClass& other) const
	{
		return this->m > other.m;
	}

	TestClass operator-(const TestClass& other) const 
	{

		TestClass c;
		c.m = this->m - other.m;
		return c;
	}
};

void DifferentClassTstcase()
{
	std::list<u64> val_str;
	RangeTree<TestClass, u64>* rangeNode = new RangeTree<TestClass, u64>();
	TestClass var( 'c');
	TestClass var2('n');
	rangeNode->insert(var, var2, 890655);
	TestClass var3('f');
	rangeNode->findAll(var3,val_str);
	rangeNode->deleteNode(var, var2);

}
int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	testMemoryLeak();
	//RangeTree<u32, std::string>* rangeNode = new RangeTree<u32, std::string>(10);
	
	/*addNodes1level(rangeNode);
    addNodes2Level(rangeNode);
	addNodes3LevelTestCase(rangeNode);
	
	singleAddLeft(rangeNode);
	singleAddRight(rangeNode);
	deleteNodesTestCase(rangeNode);
	searchTestCases(rangeNode);*/

	//DifferentClassTstcase();
	//std::cout<<"Hello World";
	//rangeNode->clear();
	_CrtDumpMemoryLeaks();
	return 0;
}

