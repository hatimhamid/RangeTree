/*
	Filename: RangeTree.h
	Author: Hatim Hamid
	Created: 26/06/2020

	This file manages the operations performed on Range Node such as Addition, Deletion and Searching.
	RangeTree can take max range as a parameter to define maximum range for a range or it can take no
	range at all meaning variable ranges and the max range will be treated as infinite. However, providing
	the max range can significantly improve the searching performance. RangeTree treats the lower bound as
	a key(unique) and manages the DS accordingly.

	Future Improvements Scope:
	1. Iterator support
	2. Support for Comparator
	3. Reverse Range
*/

#pragma once
#ifndef RANGE_NODE_H
#define RANGE_NODE_H

#include "RangeNode.h"
#include <list>
#include <Windows.h>

namespace rangetree
{

template <class K, class V>
class RangeTree
{
private:
	RangeNode<K,V>* p_head;
	unsigned __int64 m_size;
	K* _pmaxrange;
	bool _bUniformRange;

	RangeTree(const RangeTree& other) {}
public:
	RangeTree()
	{
		_bUniformRange = false;
		_pmaxrange = (K*)nullptr;
		m_size = 0;
		p_head = nullptr;
	}

	RangeTree(K range)
	{
		_bUniformRange = true;
		_pmaxrange = new K(range);
		m_size = 0;
		p_head = nullptr;
	}

	~RangeTree(void)
	{
		if(nullptr != p_head)
			delete p_head;

		p_head = nullptr;

		if(nullptr != _pmaxrange)
			delete _pmaxrange;
		_pmaxrange = (K*)nullptr;
	}

	void clear()
	{
		if(nullptr != p_head)
			delete p_head;
		p_head = nullptr;
		m_size = 0;
	}

	unsigned int getSize()
	{
		return m_size;
	}

	void insert(K lowerBound, K upperBound, V value)
	{
		if(nullptr == p_head)
		{
			p_head = createNewNode(lowerBound, upperBound, value);
		}
		else
		{
			std::list<RangeNode<K,V>**> listconcernedLowestNodesInLeftSubTree;
			insert(p_head, lowerBound, upperBound, value, (RangeNode<K,V>**)nullptr, listconcernedLowestNodesInLeftSubTree);
		}
	}

	void deleteNode(K lowerBound, K upperBound)
	{
		if(p_head)
		{
			std::list<RangeNode<K,V>**> listconcernedLowestNodesInLeftSubTree;
			p_head = deleteNode(p_head, lowerBound, (RangeNode<K,V>**)nullptr, listconcernedLowestNodesInLeftSubTree);
		}
	}

	void findAll(K searchKey, std::list<V>& searchResults)
	{
		if(p_head)
			searchAll(p_head, searchKey, &searchResults);
	}

	void setMaxRange(K range)
	{
		_bUniformRange = true;
		if(nullptr != _pmaxrange)
			delete _pmaxrange;
		_pmaxrange = (K*)nullptr;
		_pmaxrange = new(range);
	}

	bool find(K searchKey)
	{
		if(p_head)
			return search(p_head, searchKey);
	}

private:

	//Creates and return a new node and increases the size of the tree
	RangeNode<K,V>* createNewNode(K lowerBound, K upperBound, V value)
	{
		m_size++;
		return new RangeNode<K,V>(lowerBound, upperBound, value);
	}
	
	void deleteNode(RangeNode<K,V>* node)
	{
		if(node)
		{
			node->p_left = nullptr;
			node->p_right = nullptr;

			delete node; 
			m_size--;
		}
	}

	RangeNode<K,V>* insert(RangeNode<K,V>* node, K& lowerBound, K& upperBound, V& value, 
							RangeNode<K,V>** concernedLowestNodeInRightSubTree, std::list<RangeNode<K,V>**>& listconcernedLowestNodesInLeftSubTree)
	{
		if(node == nullptr)
		{
			RangeNode<K,V>* newNode = createNewNode(lowerBound, upperBound, value);
			if(concernedLowestNodeInRightSubTree)
				*concernedLowestNodeInRightSubTree = newNode;

			if(listconcernedLowestNodesInLeftSubTree.size() > 0)
			{
				auto itr = listconcernedLowestNodesInLeftSubTree.begin();
				while(itr != listconcernedLowestNodesInLeftSubTree.end())
				{
					*(*itr) = newNode;
					itr++;
				}
			}
			listconcernedLowestNodesInLeftSubTree.clear();
			return newNode;
		}

		if(lowerBound < *node->m_pLowerBound )
		{
			//Check to see if the current node has the max node in its left subtree
			if(node->p_LowestNodeinLeftSubTree == nullptr || lowerBound < *(node->p_LowestNodeinLeftSubTree->m_pLowerBound))
				listconcernedLowestNodesInLeftSubTree.push_back(&(node->p_LowestNodeinLeftSubTree));

			node->p_left = insert(node->p_left, lowerBound, upperBound, value, concernedLowestNodeInRightSubTree, listconcernedLowestNodesInLeftSubTree);
		}

		else if(lowerBound > *node->m_pLowerBound )
		{
			//Check to see if the current node has the min node in its right subtree
			if(node->p_LowestNodeInRightSubTree == nullptr || lowerBound < *(node->p_LowestNodeInRightSubTree->m_pLowerBound))
				node->p_right = insert(node->p_right, lowerBound, upperBound, value, &(node->p_LowestNodeInRightSubTree), listconcernedLowestNodesInLeftSubTree);
			else
				node->p_right = insert(node->p_right, lowerBound, upperBound, value, concernedLowestNodeInRightSubTree, listconcernedLowestNodesInLeftSubTree);
		}
		else
		{
			*node->m_pValue = value;
		}
		return node;
	}

	RangeNode<K,V>* deleteNode(RangeNode<K,V>* root, K& lowerBound, RangeNode<K,V>** concernedLowestNodeInRightSubTree, 
								std::list<RangeNode<K,V>**>& listconcernedLowestNodesInLeftSubTree)
	{
		if(root == nullptr)
			return root;

		// If the key to be deleted is smaller than the root's key, 
		// then it lies in left subtree 
		if (lowerBound < *(root->m_pLowerBound)) 
		{
			//Resolving concerns by checking if the concern passed is in the left of this sub-tree and has a single child. 
			//If yes, then the concerned LNIRS is the current node.
			if(concernedLowestNodeInRightSubTree && *((*concernedLowestNodeInRightSubTree)->m_pLowerBound) == *(root->p_left->m_pLowerBound))
			{
				*concernedLowestNodeInRightSubTree = root;
				concernedLowestNodeInRightSubTree = nullptr;
			}

			if(listconcernedLowestNodesInLeftSubTree.size() > 0 && *((*listconcernedLowestNodesInLeftSubTree.front())->m_pLowerBound) == *(root->p_left->m_pLowerBound))
			{
				auto itr = listconcernedLowestNodesInLeftSubTree.begin();
				while(itr != listconcernedLowestNodesInLeftSubTree.end())
				{
					*(*itr) = root;
					itr++;
				}
				listconcernedLowestNodesInLeftSubTree.clear();
			}

			//Identifying concerns by checking if the node to be deleted is the curr nodes HNILS. If yes, then send its concern to resolve.
			if(root ->p_LowestNodeinLeftSubTree && *(root ->p_LowestNodeinLeftSubTree->m_pLowerBound) == lowerBound)
				listconcernedLowestNodesInLeftSubTree.push_back(&(root->p_LowestNodeinLeftSubTree));

			root->p_left = deleteNode(root->p_left, lowerBound, concernedLowestNodeInRightSubTree, listconcernedLowestNodesInLeftSubTree);
		}

		// If the key to be deleted is greater than the root's key, 
		// then it lies in right subtree 
		else if (lowerBound > *(root->m_pLowerBound)) 
		{
			//Identifying concerns by checking if the node to be deleted is the curr nodes LNIRS. If yes, then send its concern to resolve.
			if(root ->p_LowestNodeInRightSubTree && *(root ->p_LowestNodeInRightSubTree->m_pLowerBound) == lowerBound)
			{
				root->p_right = deleteNode(root->p_right, lowerBound, &(root ->p_LowestNodeInRightSubTree), listconcernedLowestNodesInLeftSubTree);
			}
			else
				root->p_right = deleteNode(root->p_right, lowerBound, concernedLowestNodeInRightSubTree, listconcernedLowestNodesInLeftSubTree);
		}

		// if key is same as root's key, then this is the node 
		// to be deleted 
		else
		{
			// node with only one child or no child 
			if (root->p_left == nullptr) 
			{ 
				RangeNode<K,V>* temp = root->p_right;

				deleteNode(root);

				//If the concern still persists, then the concerned node is in the
				//left/right of the previous node and has a single child. Resolve the 
				//concern by simply updating it to the next node.
				if(concernedLowestNodeInRightSubTree)
				{
					if(temp && temp->p_LowestNodeinLeftSubTree)
						*concernedLowestNodeInRightSubTree = temp->p_LowestNodeinLeftSubTree;
					else
						*concernedLowestNodeInRightSubTree = temp;

					concernedLowestNodeInRightSubTree = nullptr;
				}
				if(listconcernedLowestNodesInLeftSubTree.size() > 0)
				{
					auto itr = listconcernedLowestNodesInLeftSubTree.begin();
					while(itr != listconcernedLowestNodesInLeftSubTree.end())
					{
						*(*itr) = temp;
						itr++;
					}
					listconcernedLowestNodesInLeftSubTree.clear();
				}

				return temp; 
			} 

			else if (root->p_right == nullptr) 
			{ 
				RangeNode<K,V>* temp = root->p_left; 
				
				deleteNode(root);

				//If the concern still persists, then the concerned node is in the
				//left/right of the previous node and has a single child. Resolve the 
				//concern by simply updating it to the next node.
				if(concernedLowestNodeInRightSubTree)
				{
					if(temp && temp->p_LowestNodeinLeftSubTree)
						*concernedLowestNodeInRightSubTree = temp->p_LowestNodeinLeftSubTree;
					else
						*concernedLowestNodeInRightSubTree = temp;

					concernedLowestNodeInRightSubTree = nullptr;
				}

				return temp; 
			} 

			//If the node has come here, that means the node is not the LNIRS and HNIRS of any of the above nodes.

			// node with two children: Get the LNIRS
			RangeNode<K,V>* temp = root->p_LowestNodeInRightSubTree;

			// Copy the inorder successor's content to this node 
			*root->m_pLowerBound = *temp->m_pLowerBound; 
			*root->m_pUpperBound = *temp->m_pUpperBound; 
			*root->m_pValue = *temp->m_pValue; 

			// Delete the inorder successor. Also send the its LNIRS as a concern as it must be affected.
			root->p_right = deleteNode(root->p_right, *temp->m_pLowerBound, &root->p_LowestNodeInRightSubTree, listconcernedLowestNodesInLeftSubTree); 
		}
		return root;
	}

	void searchAll(RangeNode<K,V>* root, const K& searchKey, std::list<V>* searchResults)
	{
		if(root == nullptr)
			return ;

		if(searchKey < *root->m_pLowerBound)
		{
			if(root ->p_LowestNodeinLeftSubTree)
			{
				//Go left only if search key is less than LNILS or search key is in
				//the range of LNILS
				if(searchKey >= *(root->p_LowestNodeinLeftSubTree->m_pLowerBound))
					searchAll(root->p_left, searchKey, searchResults);
			}
		}

		else
		{
			if(searchKey > *root->m_pLowerBound && searchKey < *root->m_pUpperBound)
			{
				//Filter matched
				searchResults->push_back(*(root->m_pValue));
			}

			if(_bUniformRange)
			{
				if(searchKey - *root->m_pLowerBound  < *_pmaxrange)
					searchAll(root->p_left, searchKey, searchResults);
				
				//Check if the search key is greater or equal to LNIRS. If yes, then go right.
				if(root ->p_LowestNodeInRightSubTree && searchKey >= *(root ->p_LowestNodeInRightSubTree->m_pLowerBound))
					searchAll(root->p_right, searchKey, searchResults);
			}
			else
			{
				//Max Range is 0, then we have to search in the left as well as right
				searchAll(root->p_left, searchKey, searchResults);
				searchAll(root->p_right, searchKey, searchResults);
			}
			
		}
	}

	bool search(RangeNode<K,V>* root, const K& searchKey)
	{
		if(root == nullptr)
			return false;

		if(searchKey < *root->m_pLowerBound)
		{
			if(root ->p_LowestNodeinLeftSubTree)
			{
				if(searchKey >= *(root->p_LowestNodeinLeftSubTree->m_pLowerBound))
					return search(root->p_left, searchKey);
			}
		}

		else if(searchKey >= *root->m_pLowerBound)
		{
			if(searchKey > *root->m_pLowerBound && searchKey < *root->m_pUpperBound)
			{
				//Filter matched
				return true;
			}
			else
			{
				if(_bUniformRange)
				{
					if(searchKey - *root->m_pLowerBound  < *_pmaxrange)
						return search(root->p_left, searchKey);

					if(root ->p_LowestNodeInRightSubTree && searchKey >= *(root ->p_LowestNodeInRightSubTree->m_pLowerBound))
						return search(root->p_right, searchKey);
				}
				else
				{
					return search(root->p_left, searchKey);
					return search(root->p_right, searchKey);
				}
			}
		}
		return false;
	}
};

}
#endif