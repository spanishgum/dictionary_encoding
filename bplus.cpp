#include "bplus.h"
#include "trie.h"
#include <iostream>


template <typename Tkey, typename Tdat>
Bplus<Tkey, Tdat>::Bplus() 
{
	headLeaf = tailLeaf = NULL;
	root = NULL;
}

template <typename Tkey, typename Tdat>
Bplus<Tkey, Tdat>::~Bplus() 
{
	this->clear();
}


template <typename Tkey, typename Tdat>
typename Bplus<Tkey, Tdat>::Node *Bplus<Tkey, Tdat>::createNode() 
{

	return NULL;
}

template <typename Tkey, typename Tdat>
void Bplus<Tkey, Tdat>::merge(Bplus<Tkey, Tdat>::Node *N, Bplus<Tkey, Tdat>::Node *M) 
{

}

template <typename Tkey, typename Tdat>
void Bplus<Tkey, Tdat>::split(Bplus<Tkey, Tdat>::innerNode *iNode, Tkey *_newKey, Node **_newInner, unsigned int addSlot) 
{
	unsigned int mid = (iNode->slots_used >> 1);
	innerNode *newInner = new innerNode();

	if (addSlot <= mid && mid > iNode->slots_used - (mid + 1))
		mid--;
	
	newInner->slots_used = iNode->slots_used - (mid + 1);
	
	std::copy(iNode->keys + mid + 1, iNode->keys + iNode->slots_used,
		newInner->keys);
	std::copy(iNode->pointers + mid + 1, iNode->pointers + iNode->slots_used + 1,
		newInner->pointers);
	
	iNode->slots_used = mid;
	
	*_newKey = iNode->keys[mid];
	*_newInner = newInner;
}

template <typename Tkey, typename Tdat>
void Bplus<Tkey, Tdat>::split(Bplus<Tkey, Tdat>::leafNode *lNode, Tkey *_newKey, Node **_newLeaf) 
{
	unsigned int mid = (lNode->slots_used >> 1);
	leafNode *newLeaf = new leafNode();
	
	newLeaf->slots_used = lNode->slots_used - mid;
	newLeaf->next = lNode->next;
	
	if (newLeaf->next == NULL) 
	{
		this->tailLeaf = newLeaf;
	}
	else 
	{
		std::cout << newLeaf->next;
		newLeaf->next->prev = newLeaf;
	}
	
	std::copy(lNode->keys + mid, lNode->keys + lNode->slots_used,
		newLeaf->keys);
	std::copy(lNode->pointers + mid, lNode->pointers + lNode->slots_used,
		newLeaf->pointers);

	lNode->slots_used = mid;
	lNode->next = newLeaf;
	newLeaf->prev = lNode;

	*_newKey = lNode->keys[lNode->slots_used - 1];
	*_newLeaf = newLeaf;
}

template <typename Tkey, typename Tdat>
void Bplus<Tkey, Tdat>::clear_recursive(Bplus<Tkey, Tdat>::Node *N) 
{
	if (!N)
		return;
	else if (N->isleaf) 
	{
		Bplus::leafNode *lNode = static_cast<Bplus::leafNode *>(N);
		for (unsigned int slot = 0; slot < lNode->slots_used; ++slot) 
		{
			delete static_cast<Tdat *>(lNode->pointers[slot]);
		}
		delete static_cast<Bplus<Tkey, Tdat>::leafNode *>(N);
	}
	else 
	{
		Bplus::innerNode *iNode = static_cast<Bplus<Tkey, Tdat>::innerNode *>(N);
		for (unsigned int slot = 0; slot < iNode->slots_used; ++slot) 
		{
			clear_recursive(static_cast<Bplus<Tkey, Tdat>::Node *>(iNode->pointers[slot]));
			delete static_cast<Bplus<Tkey, Tdat>::Node *>(iNode->pointers[slot]);
		}
		delete static_cast<Bplus<Tkey, Tdat>::innerNode *>(N);
	}
}

template <typename Tkey, typename Tdat>
typename Bplus<Tkey, Tdat>::Node * Bplus<Tkey, Tdat>::insert_recursive(Node *N, const Tkey &key, Tdat &dat, Tkey *splitKey, Node **splitNode) 
{

	if (N == NULL) 
		std::cerr << "Insert recursive: Null Node * found\n";
	
	if (!N->isleaf) 
	{
		innerNode *iNode = static_cast<innerNode *>(N);
		Tkey newKey = Tkey();
		Node *newChild = NULL;
		
		int slot = lower_key_idx(N, key);
		Node *recurse_result = insert_recursive(static_cast<Node *>(iNode->pointers[slot]), key, dat, &newKey, &newChild);
		
		if (newChild) 
		{	
			if (iNode->isfull()) 
			{	
				split(iNode, splitKey, splitNode, slot);
				if (slot == iNode->slots_used + 1 && iNode->slots_used < (*splitNode)->slots_used)
				{
					innerNode *splitInner = static_cast<innerNode *>(*splitNode);
					iNode->keys[iNode->slots_used] = *splitKey;
					iNode->pointers[iNode->slots_used + 1] = splitInner->pointers[0];
					iNode->slots_used++;
					splitInner->pointers[0] = newChild;
					*splitKey = newKey;
					return recurse_result;
				}
				else if (slot >= iNode->slots_used + 1) 
				{
					slot -= iNode->slots_used + 1;
					iNode = static_cast<innerNode *>(*splitNode);
				}
			}
			
			std::copy_backward(iNode->keys + slot, iNode->keys + iNode->slots_used,
				iNode->keys + iNode->slots_used + 1);
			std::copy_backward(iNode->pointers + slot, iNode->pointers + iNode->slots_used + 1,
				iNode->pointers + iNode->slots_used + 2);
			
			iNode->keys[slot] = newKey;
			iNode->pointers[slot + 1] = newChild;
			iNode->slots_used++;
		}
		
		return recurse_result;
	}
	else 
	{
		leafNode *lNode = static_cast<leafNode *>(N);
		int slot = lower_key_idx(N, key);
		
		if (slot < lNode->slots_used && key == lNode->keys[slot])
			return lNode;
		
		if (lNode->isfull()) 
		{

			// std::cout << "splitting leaf node . . .\n";

			split(lNode, splitKey, splitNode);
			
			
			if (slot >= lNode->slots_used) 
			{
				slot -= lNode->slots_used;
				lNode = static_cast<leafNode *>(*splitNode);
			}
		}
		
		std::copy_backward(lNode->keys + slot, lNode->keys + lNode->slots_used,
				lNode->keys + lNode->slots_used + 1);
		std::copy_backward(lNode->pointers + slot, lNode->pointers + lNode->slots_used,
			lNode->pointers + lNode->slots_used + 1);

		lNode->keys[slot] = key;
		lNode->pointers[slot] = &dat;
		lNode->slots_used++;

		if (splitNode && lNode != *splitNode && slot == lNode->slots_used - 1) 
		{
			*splitKey = key;
		}
		
		return lNode;
	}
}

template <typename Tkey, typename Tdat>
void Bplus<Tkey, Tdat>::traverse(Bplus<Tkey, Tdat>::Node *N) 
{
	if (N->isleaf) 
	{
		std::cout << N->slots_used << "|";
		for (int key = 0; key < N->slots_used; ++key) 
		{
			std::cout << N->keys[key] << ",";
		}
		std::cout << "|\n";
	}
	else
		for (int key = 0; key < N->slots_used; ++key)
			traverse(static_cast<Bplus<Tkey, Tdat>::Node *>(N->pointers[key]));
}

template <typename Tkey, typename Tdat>
Tdat * Bplus<Tkey, Tdat>::find(Tkey key) 
{
	Node *N = this->root;
	if (!N) return NULL;
	
	while (!N->isleaf) 
	{
		int slot = lower_key_idx(N, key);
		N = static_cast<Bplus<Tkey, Tdat>::Node *>(N->pointers[slot]);
	}
	
	int slot = lower_key_idx(N, key);
	
	return static_cast<Tdat *>(N->pointers[slot]);
}

template <typename Tkey, typename Tdat>
typename Bplus<Tkey, Tdat>::Node *Bplus<Tkey, Tdat>::insert(Tkey &key, Tdat &dat) 
{
	Node *newChild = NULL;
	Tkey newKey = Tkey();
	
	if (this->root == NULL) 
	{
		this->root = new leafNode();
		this->headLeaf = this->tailLeaf = this->root;
	}
	
	Node *return_val = insert_recursive(this->root, key, dat, &newKey, &newChild);
	
	if (newChild) 
	{	
		innerNode *newRoot = new innerNode();
		newRoot->keys[0] = newKey;
		newRoot->pointers[0] = this->root;
		newRoot->pointers[1] = newChild;
		newRoot->slots_used = 1;
		this->root = newRoot;	
	}
	
	return return_val;
}

template <typename Tkey, typename Tdat>
int Bplus<Tkey, Tdat>::remove(Tkey) 
{

	return 0;
}

template <typename Tkey, typename Tdat>
void Bplus<Tkey, Tdat>::clear() 
{
	clear_recursive(this->root);
}

template <typename Tkey, typename Tdat>
void Bplus<Tkey, Tdat>::show() 
{
	this->traverse(this->root);
}

template class Bplus<int, int>;
template class Bplus<int, Trie::Node>;
template class Bplus<unsigned int, Trie::Node>;
