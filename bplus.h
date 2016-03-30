#ifndef BPLUS_H
#define BPLUS_H

#include <cstdlib>

template <typename T>
class Bplus {
	private:

		struct Node {
			struct Node *parent;
			T *keys;
			void **pointers;
			unsigned int slots_used;
			bool isleaf;
			inline Node(int k_value, Node *p = NULL)
				: parent(p) {
				keys = new T[k_value];
				pointers = new void *[k_value + 1];
			}
		};

		struct innerNode : public Node {
			inline innerNode()
				: Node(1) {}
		};

		struct leafNode : public Node {
			struct leafNode *prev, *next;
			inline leafNode()
				: Node(0), prev(NULL), next(NULL) {}
		};

		int order, size;
		Node *root;
		leafNode *headLeaf, *tailLeaf;

		Node *createNode();
		void merge(Node *, Node *);
		void split(Node *);
		void clear_recursive(Node *);
		void traverse(Node *);

	public:
		Bplus(int = 3);
		~Bplus();
		Node *find();
		Node *insert();
		int remove(T);
		void clear();
		void show();
};


#endif