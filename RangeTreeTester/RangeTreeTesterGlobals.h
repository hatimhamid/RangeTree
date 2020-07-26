#pragma once
#ifndef RANGE_TREE_TESTER_GLOBALS_H
#define RANGE_TREE_TESTER_GLOBALS_H

typedef unsigned int u32;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned long long u64;

class RangeTreeTesterGlobals
{
private:
	RangeTreeTesterGlobals(void)
	{
	}
public:
	RangeTreeTesterGlobals* GetInstance()
	{
		static RangeTreeTesterGlobals* rangeTreeTesterGlobals = new RangeTreeTesterGlobals();
		return rangeTreeTesterGlobals;
	}
	~RangeTreeTesterGlobals(void)
	{
	}
};

#if 0
/*
	Function Name: insert(RangeNode* (current node), class (lower bound), class (upper bound), class (value),
							RangeNode**, RangeNode**)
	Return Value: RangeNode* (new created node)

	Function to insert a new Node based on lower bound(Key) comparision b/w the nodes and returns
	the newly created node. Returns null when the key already exists. It also simultaneously updates
	the concerned nodes Lowest Node in Right SubTree(LNIRS) and the Highest Node in Left SubTree(HNILS).
	It doesn't add duplicate keys!
	*/
	RangeNode<K,V>* insert(RangeNode<K,V>* node, K& lowerBound, K& upperBound, V& value, 
							RangeNode<K,V>** concernedLowestNodeInRightSubTree, RangeNode<K,V>** concernedHighestNodeInLeftSubTree)
	{
		if(node == nullptr)
		{
			RangeNode<K,V>* newNode = createNewNode(lowerBound, upperBound, value);
			if(concernedLowestNodeInRightSubTree)
				*concernedLowestNodeInRightSubTree = newNode;
			if(concernedHighestNodeInLeftSubTree)
				*concernedHighestNodeInLeftSubTree = newNode;

			return newNode;
		}

		if(lowerBound < *node->m_pLowerBound )
		{
			//Check to see if the current node has the max node in its left subtree
			if(node->p_HighestNodeinLeftSubTree == nullptr || lowerBound > *(node->p_HighestNodeinLeftSubTree->m_pLowerBound))
				node->p_left = insert(node->p_left, lowerBound, upperBound, value, concernedLowestNodeInRightSubTree, &(node->p_HighestNodeinLeftSubTree));
			else
				node->p_left = insert(node->p_left, lowerBound, upperBound, value, concernedLowestNodeInRightSubTree, concernedHighestNodeInLeftSubTree);
		}

		else if(lowerBound > *node->m_pLowerBound )
		{
			//Check to see if the current node has the min node in its right subtree
			if(node->p_LowestNodeInRightSubTree == nullptr || lowerBound < *(node->p_LowestNodeInRightSubTree->m_pLowerBound))
				node->p_right = insert(node->p_right, lowerBound, upperBound, value, &(node->p_LowestNodeInRightSubTree), concernedHighestNodeInLeftSubTree);
			else
				node->p_right = insert(node->p_right, lowerBound, upperBound, value, concernedLowestNodeInRightSubTree, concernedHighestNodeInLeftSubTree);
		}
		else
		{
			*node->m_pValue = value;
		}
		return node;
	}

	/*
	Function Name: deleteNode(RangeNode* (current node), class (key to delete), RangeNode**, RangeNode**)
	Return Value: RangeNode* (updated node)

	Function deletes the passed key in the tree. It also simultaneously updates the affected LNIRS and HNILS due to its deletion.
	*/
	RangeNode<K,V>* deleteNode(RangeNode<K,V>* root, K& lowerBound, RangeNode<K,V>** concernedLowestNodeInRightSubTree, 
								RangeNode<K,V>** concernedHighestNodeInLeftSubTree)
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

			//Identifying concerns by checking if the node to be deleted is the curr nodes HNILS. If yes, then send its concern to resolve.
			if(root ->p_HighestNodeinLeftSubTree && *(root ->p_HighestNodeinLeftSubTree->m_pLowerBound) == lowerBound)
			{
				root->p_left = deleteNode(root->p_left, lowerBound, concernedLowestNodeInRightSubTree, &(root ->p_HighestNodeinLeftSubTree));
			}
			else
				root->p_left = deleteNode(root->p_left, lowerBound, concernedLowestNodeInRightSubTree, concernedHighestNodeInLeftSubTree);
		}

		// If the key to be deleted is greater than the root's key, 
		// then it lies in right subtree 
		else if (lowerBound > *(root->m_pLowerBound)) 
		{
			//Resolving concerns by checking if the concern passed is in the right of this sub-tree and has a single child. 
			//If yes, then the concerned HNILS is the current node.
			if(concernedHighestNodeInLeftSubTree && *((*concernedHighestNodeInLeftSubTree)->m_pLowerBound) == *(root->p_right->m_pLowerBound))
			{
				*concernedHighestNodeInLeftSubTree = root;
				concernedHighestNodeInLeftSubTree = nullptr;
			}

			//Identifying concerns by checking if the node to be deleted is the curr nodes LNIRS. If yes, then send its concern to resolve.
			if(root ->p_LowestNodeInRightSubTree && *(root ->p_LowestNodeInRightSubTree->m_pLowerBound) == lowerBound)
			{
				root->p_right = deleteNode(root->p_right, lowerBound, &(root ->p_LowestNodeInRightSubTree), concernedHighestNodeInLeftSubTree);
			}
			else
				root->p_right = deleteNode(root->p_right, lowerBound, concernedLowestNodeInRightSubTree, concernedHighestNodeInLeftSubTree);
		}

		// if key is same as root's key, then this is the node 
		// to be deleted 
		else
		{
			// node with only one child or no child 
			if (root->p_left == nullptr) 
			{ 
				RangeNode<K,V>* temp = root->p_right;
				//doing left and right pointers to null as we are deleting the node
				root->p_left = nullptr;
				root->p_right = nullptr;

				delete root; 

				//If the concern still persists, then the concerned node is in the
				//left/right of the previous node and has a single child. Resolve the 
				//concern by simply updating it to the next node.
				if(concernedHighestNodeInLeftSubTree)
				{
					*concernedHighestNodeInLeftSubTree = temp;
					concernedHighestNodeInLeftSubTree = nullptr;
				}
				else if(concernedLowestNodeInRightSubTree)
				{
					*concernedLowestNodeInRightSubTree = temp;
					concernedLowestNodeInRightSubTree = nullptr;
				}

				m_size--;
				return temp; 
			} 

			else if (root->p_right == nullptr) 
			{ 
				RangeNode<K,V>* temp = root->p_left; 
				//doing left and right pointers to null as we are deleting the node
				root->p_left = nullptr;
				root->p_right = nullptr;

				delete root; 

				//If the concern still persists, then the concerned node is in the
				//left/right of the previous node and has a single child. Resolve the 
				//concern by simply updating it to the next node.
				if(concernedHighestNodeInLeftSubTree)
				{
					*concernedHighestNodeInLeftSubTree = temp;
					concernedHighestNodeInLeftSubTree = nullptr;
				}
				else if(concernedLowestNodeInRightSubTree)
				{
					*concernedLowestNodeInRightSubTree = temp;
					concernedLowestNodeInRightSubTree = nullptr;
				}

				m_size--;
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
			root->p_right = deleteNode(root->p_right, *temp->m_pLowerBound, &root->p_LowestNodeInRightSubTree, concernedHighestNodeInLeftSubTree); 
		}
		return root;
	}

	/*
	Function name: searchAll(RangeNode* (current node), class (search key), list* (search results))

	Function searches for the key in the tree that satisfies the range and returns the nodes values.
	The search key is in the range if it satisfies the condition: [lower bound, upper bound).
	*/
	void searchAll(RangeNode<K,V>* root, const K& searchKey, std::list<V>* searchResults)
	{
		if(root == nullptr)
			return ;

		if(searchKey < *root->m_pLowerBound)
		{
			if(m_MaxRange == 0)
				searchAll(root->p_left, searchKey, searchResults);
			else
			{
				if(root ->p_HighestNodeinLeftSubTree)
				{
					//Go left only if search key is less than HNILS or search key is in
					//the range of HNILS
					if(searchKey < *(root->p_HighestNodeinLeftSubTree->m_pLowerBound) || 
						(searchKey - *(root->p_HighestNodeinLeftSubTree->m_pLowerBound)) < m_MaxRange)
						searchAll(root->p_left, searchKey, searchResults);
				}
			}
			
		}

		else if(searchKey >= *root->m_pLowerBound)
		{
			bool isMatched = searchKey < *root->m_pUpperBound; 
			if(isMatched)
			{
				//Filter matched
				searchResults->push_back(*(root->m_pValue));
			}

			if(m_MaxRange == 0)
			{
				//Max Range is 0, then we have to search in the left as well as right
				searchAll(root->p_left, searchKey, searchResults);
				searchAll(root->p_right, searchKey, searchResults);
			}
			else
			{
				if(isMatched)
					searchAll(root->p_left, searchKey, searchResults);
				
				//Check if the search key is greater or equal to LNIRS. If yes, then go right.
				if(root ->p_LowestNodeInRightSubTree && searchKey >= *(root ->p_LowestNodeInRightSubTree->m_pLowerBound))
					searchAll(root->p_right, searchKey, searchResults);
			}
			
		}
	}
#endif

#endif