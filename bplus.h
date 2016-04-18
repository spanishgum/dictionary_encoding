#ifndef BPLUS_H
#define BPLUS_H

#include <cstdlib>

#define CACHE_LINE_SIZE 256
#define Bmax(a, b) ((a) > (b) ? (a) : (b))

template <typename Tkey, typename Tdat>
class Bplus 
{
	protected:

		static const unsigned int 
			max_slots = Bmax(8, CACHE_LINE_SIZE / (sizeof(Tkey) + sizeof(void *))),
			// max_slots = 4,
			min_slots = (max_slots / 2);

		struct Node
		{
			Tkey *keys;
			void **pointers;
			unsigned int slots_used;
			bool isleaf;
			
			inline Node(bool _isleaf) 
			{
				keys = new Tkey[max_slots];
				pointers = new void *[max_slots + 1];
				isleaf = _isleaf;
				slots_used = 0;
		
				for (int i = 0; i < max_slots; ++i) 
				{
					keys[i] = Tkey();
					pointers[i] = NULL;
				}
				
				pointers[max_slots] = NULL;
			}
			
			inline bool isfull() const 
			{
				return (slots_used == max_slots);
			}
			
			inline bool isfew() const 
			{
				return (slots_used <= min_slots);
			}
			
			inline bool isunderflow() const 
			{
				return (slots_used < min_slots);
			}
		};

		struct innerNode : public Node 
		{
			inline innerNode()
				: Node(false) {}
		};

		struct leafNode : public Node 
		{
			struct leafNode *prev, *next;
			inline leafNode()
				: Node(true), prev(NULL), next(NULL) {}
		};		
		
		Node *root;
		Node *headLeaf, *tailLeaf;

		inline int lower_key_idx(const Node *N, const Tkey& key) const 
		{
			int lo = 0;
			while (lo < N->slots_used && (N->keys[lo] < key)) 
				++lo;
			return lo;
		}
		
		inline int upper_key_idx(const Node *N, const Tkey& key) const 
		{
			int lo = N->slots_used;
			while (lo < N->slots_used && (N->keys[lo] <= key)) 
				++lo;
			return lo;
		}

		enum remove_flags
		{
			ok = 0,
			key_not_found = 1,
			update_last_key = 2,
			fix_merge = 4
		};
		
		struct remove_result
		{
			remove_flags flags;
			Tkey last_key;
			
			inline explicit remove_result(remove_flags f = ok)
				: flags(f), last_key() { }
			
			inline remove_result(remove_flags f, const Tkey &k)
				: flags(f), last_key(k) { }
				
			inline bool has(remove_flags f) const
			{
				return (flags & f) != 0;
			}
			
			inline remove_result& operator |= (const remove_result& other)
			{
				flags = remove_flags(flags | other.flags);
				
				if (other.has(update_last_key))
					last_key = other.last_key;
					
				return *this;
			}
		};
		
		void split_iNodes(innerNode *, Tkey *, Node **, unsigned int);
		void split_lNodes(leafNode *, Tkey *, Node **);
		
		Node *insert_recursive(Node *, const Tkey&, Tdat&, Tkey *, Node **);
		
		remove_result merge_iNodes(innerNode *, Node *, Node *, unsigned int); 
		remove_result merge_lNodes(leafNode *, leafNode *, Node *); 
		
		remove_result remove_recursive(Tkey, Node *, Node *, Node *, Node *, Node *, Node *, unsigned int);
		
		remove_result shift_lNodeL(Node *, Node *, Node *, unsigned int);
		remove_result shift_lNodeR(Node *, Node *, Node *, unsigned int);
		
		remove_result shift_iNodeL(Node *, Node *, Node *, unsigned int);
		remove_result shift_iNodeR(Node *, Node *, Node *, unsigned int);
		
		void clear_recursive(Node *);
		void traverse(Node *);

	public:
		
		Bplus();
		~Bplus();
		
		Node *insert(Tkey &, Tdat &);
		remove_result remove(Tkey);

		Tdat *find(Tkey);

		void clear();
		void show();
};


#endif