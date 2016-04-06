#ifndef BPLUS_H
#define BPLUS_H

#include <cstdlib>

#define CACHE_LINE_SIZE 256
#define Bmax(a, b) ((a) > (b) ? (a) : (b))

template <typename Tkey, typename Tdat>
class Bplus {
	private:
		static const unsigned int 
			max_slots = Bmax(8, CACHE_LINE_SIZE / (sizeof(Tkey) + sizeof(void *))),
			min_slots = (max_slots / 2);

		struct Node {
			Tkey *keys;
			void **pointers;
			unsigned int slots_used;
			bool isleaf;
			inline Node(bool _isleaf) {
				keys = new Tkey[max_slots];
				pointers = new void *[max_slots + 1];
				isleaf = _isleaf;
			}
			inline bool isfull() const {
				return (slots_used == max_slots);
			}
			inline bool isfew() const {
				return (slots_used <= min_slots);
			}
			inline bool isunderflow() const {
				return (slots_used < min_slots);
			}
		};

		struct innerNode : public Node {
			inline innerNode()
				: Node(false) {}
		};

		struct leafNode : public Node {
			struct leafNode *prev, *next;
			inline leafNode()
				: Node(true), prev(NULL), next(NULL) {}

		};		
		
		Node *root;
		Node *headLeaf, *tailLeaf;

		Node *createNode();
		inline int lower_key_idx(const Node *N, const Tkey& key) const {
			return 0;
		}
		inline int upper_key_idx(const Node *N, const Tkey& key) const {
			return 0;
		}
		
		void merge(Node *, Node *);
		void split(Node *);
		void clear_recursive(Node *);
		Node *insert_recursive(Node *, Tkey, Tdat);
		void traverse(Node *);

	public:
		Bplus();
		~Bplus();
		Node *find(Tkey);
		Node *insert(Tkey, Tdat);
		int remove(Tkey);
		void clear();
		void show();
};


#endif