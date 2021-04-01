/*
	Filename: RangeNode.h
	Author: Hatim Hamid
	Created: 26/06/2020

	This file defines the skeleton of the RangeNode class.
*/

#pragma once
#ifndef RANGE_NODE_H
#define RANGE_NODE_H

namespace rangetree
{

template <class K, class V>

class RangeNode
{
private:
	RangeNode(const RangeNode& other){}

public:

	K* m_pLowerBound;
	K* m_pUpperBound;
	V* m_pValue; 

	RangeNode* p_left;
	RangeNode* p_right;

	RangeNode* p_LowestNodeInRightSubTree;
	RangeNode* p_LowestNodeinLeftSubTree;

	RangeNode(K lowerBound, K upperBound, V value)
	{
		m_pLowerBound = new K(lowerBound);
		m_pUpperBound = new K(upperBound);
		m_pValue = new V(value);

		p_left = nullptr;
		p_right = nullptr;
		p_LowestNodeInRightSubTree = nullptr;
		p_LowestNodeinLeftSubTree = nullptr;
	}

	~RangeNode(void)
	{
		if(nullptr != p_left)
			delete p_left;
		if(nullptr != p_right)
			delete p_right;

		delete m_pLowerBound;
		delete m_pUpperBound;
		delete m_pValue;

		p_left = nullptr;
		p_right = nullptr;
		p_LowestNodeInRightSubTree = nullptr;
		p_LowestNodeinLeftSubTree = nullptr;
		m_pLowerBound = nullptr;
		m_pUpperBound = nullptr;
		m_pValue = nullptr;
	}
};

}
#endif