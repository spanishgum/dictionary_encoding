#include "bplus.h"
#include <iostream>


template <typename Tkey, typename Tdat>
Bplus<Tkey, Tdat>::Bplus() {
	headLeaf = tailLeaf = NULL;
	root = NULL;
}

template <typename Tkey, typename Tdat>
Bplus<Tkey, Tdat>::~Bplus() {
	this->clear();
}


template <typename Tkey, typename Tdat>
typename Bplus<Tkey, Tdat>::Node *Bplus<Tkey, Tdat>::createNode() {

	return NULL;
}

template <typename Tkey, typename Tdat>
void Bplus<Tkey, Tdat>::merge(Bplus<Tkey, Tdat>::Node *N, Bplus<Tkey, Tdat>::Node *M) {

}

template <typename Tkey, typename Tdat>
void Bplus<Tkey, Tdat>::split(Bplus<Tkey, Tdat>::Node *N) {

}

template <typename Tkey, typename Tdat>
void Bplus<Tkey, Tdat>::clear_recursive(Bplus<Tkey, Tdat>::Node *N) {
	if (!N)
		return;
	else if (N->isleaf) {
		Bplus::leafNode *lNode = static_cast<Bplus::leafNode *>(N);
		for (unsigned int slot = 0; slot < lNode->slots_used; ++slot) {
			delete static_cast<Tkey*>(lNode->pointers[slot]);
		}
		delete static_cast<Bplus<Tkey, Tdat>::leafNode *>(N);
	}
	else {
		Bplus::innerNode *iNode = static_cast<Bplus<Tkey, Tdat>::innerNode *>(N);
		for (unsigned int slot = 0; slot < iNode->slots_used; ++slot) {
			clear_recursive(static_cast<Bplus<Tkey, Tdat>::Node *>(iNode->pointers[slot]));
			delete static_cast<Bplus<Tkey, Tdat>::Node *>(iNode->pointers[slot]);
		}
		delete static_cast<Bplus<Tkey, Tdat>::innerNode *>(N);
	}
}

template <typename Tkey, typename Tdat>
typename Bplus<Tkey, Tdat>::Node * Bplus<Tkey, Tdat>::insert_recursive(Node *N, Tkey key, Tdat dat) {
	if (!N->isleaf) {
		innerNode *iNode = static_cast<innerNode *>(N);
		Node *newNode = NULL;
		int slot = lower_key_idx(N);
	}
	else {
		
	}
}

template <typename Tkey, typename Tdat>
void Bplus<Tkey, Tdat>::traverse(Bplus<Tkey, Tdat>::Node *N) {
	if (N->isleaf) {
		for (int key = 0; key < N->slots_used; ++key)
			std::cout << N->pointers << " ";
		std::cout << std::endl;
	}
	else
		for (int key = 0; key < N->slots_used; ++key)
			traverse(static_cast<Bplus<Tkey, Tdat>::Node *>(N->pointers[key]));
}

template <typename Tkey, typename Tdat>
typename Bplus<Tkey, Tdat>::Node * Bplus<Tkey, Tdat>::find(Tkey key) {
	Node *N = this->root;
	if (!N) return NULL;
	while (!N->isleaf) {
		int slot = lower_key_idx(N, key);
		N = static_cast<Bplus<Tkey, Tdat>::Node *>(N->pointers[slot]);
	}
	// int slot = lower_key_idx(N, key);
	// return N->pointers[slot];
	return NULL;
}

template <typename Tkey, typename Tdat>
typename Bplus<Tkey, Tdat>::Node *Bplus<Tkey, Tdat>::insert(Tkey key, Tdat dat) {
	Node *N = this->root;
	if (N == NULL) {
		this->root = this->headLeaf = this->tailLeaf = new Node(true);
	}
	insert_recursive(N);
	return NULL;
}

template <typename Tkey, typename Tdat>
int Bplus<Tkey, Tdat>::remove(Tkey) {

	return 0;
}

template <typename Tkey, typename Tdat>
void Bplus<Tkey, Tdat>::clear() {
	clear_recursive(this->root);
}

template <typename Tkey, typename Tdat>
void Bplus<Tkey, Tdat>::show() {
	this->traverse(this->root);
}

typedef void* data;
template class Bplus<int, data>;