#include "bplus.h"
#include <iostream>


template <typename T>
Bplus<T>::Bplus(int B_order) {
	if (B_order < 3) {
		std::cerr << "Warning: B+ tree must have a K value of at least 3.\n";
		throw;
	}
	this->order = B_order;
}

template <typename T>
Bplus<T>::~Bplus() {
	this->clear();
}


template <typename T>
typename Bplus<T>::Node *Bplus<T>::createNode() {

	return NULL;
}

template <typename T>
void Bplus<T>::merge(Bplus<T>::Node *N, Bplus<T>::Node *M) {

}

template <typename T>
void Bplus<T>::split(Bplus<T>::Node *N) {

}

template <typename T>
void Bplus<T>::clear_recursive(Bplus<T>::Node *N) {
	if (!N)
		return;
	else if (N->isleaf) {
		Bplus::leafNode *l = static_cast<Bplus::leafNode *>(N);
		for (unsigned int slot = 0; slot < l->slots_used; ++slot) {
			// delete slot
		}
	}
	else {
		Bplus::innerNode *i = static_cast<Bplus<T>::innerNode *>(N);
		for (unsigned int slot = 0; slot < i->slots_used; ++slot) {
			clear_recursive(static_cast<Bplus<T>::Node *>(i->pointers[slot]));
			delete static_cast<Bplus<T>::Node *>(i->pointers[slot]);
		}
	}
}

template <typename T>
void Bplus<T>::traverse(Bplus<T>::Node *N) {
	if (N->isleaf) {
		for (int key = 0; key < N->slots_used; ++key)
			std::cout << N->pointers << " ";
		std::cout << std::endl;
	}
	else
		for (int key = 0; key < N->slots_used; ++key)
			traverse(static_cast<Bplus<T>::Node *>(N->pointers[key]));
}

template <typename T>
typename Bplus<T>::Node * Bplus<T>::find() {

	return NULL;
}

template <typename T>
typename Bplus<T>::Node *Bplus<T>::insert() {

	return NULL;
}

template <typename T>
int Bplus<T>::remove(T) {

	return 0;
}

template <typename T>
void Bplus<T>::clear() {
	clear_recursive(this->root);
}

template <typename T>
void Bplus<T>::show() {
	this->traverse(this->root);
}


template class Bplus<int>;