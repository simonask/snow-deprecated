#ifndef RBTREE_H_9JSHAD
#define RBTREE_H_9JSHAD

#include "Basic.h"

namespace snow {
	template <typename T>
	class SimpleComparator {
	public:
		int operator()(T a, T b) const {
			if (a == b)
				return 0;
			else if (a > b)
				return 1;
			else
				return -1;
		}
	};

	class SimpleAllocator {
	public:
		void* alloc(size_t sz) { return new byte[sz]; }
		void free(void* ptr) { delete [] (byte*)ptr; }
	};

	/*
		snow::RBTree is a GC-friendly implementation of a Red/Black Binary Tree.
		Its main use is for hash map implementations, but it is generic.

		Uses Julienne Walker style tree nodes.

		Implementation of insert and erase lends heavily from the following resources:

			http://en.literateprograms.org/Red-black_tree_(C)
			http://en.wikipedia.org/wiki/Red-black_tree
	*/
	template <typename K, typename V, typename Comparator = SimpleComparator<K>, typename Allocator = SimpleAllocator>
	class RBTree {
	public:
		enum { LEFT = 0, RIGHT = 1 };
		enum Color { BLACK = 0, RED = 1 };

		// Needs to be public for GC purposes.
		struct Node {
			Color color;
			K key;
			V value;
			Node* parent;
			union {
				Node* children[2];
				struct {
					Node* left;
					Node* right;
				};
			};
		private:
			void operator delete(void*) {}
			void* operator new(size_t) { return NULL; }
		};
		
		template <typename NodeType = Node*>
		class AnyIterator {
			friend class RBTree;
		protected:
			NodeType m_Current;

			explicit AnyIterator(NodeType root) : m_Current(root) {}

			NodeType select_next(NodeType current, NodeType backtracking_from = NULL) const {
				if (!current)
					return NULL;

				if (backtracking_from) {
					if (backtracking_from == current->left) {
						if (current->right)
							return current->right;
						else
							return select_next(current->parent, current);
					} else if (backtracking_from == current->right)
						return select_next(current->parent, current);
					else
						return NULL; // end
				}

				if (current->left)
					return current->left;
				else if (current->right)
					return current->right;
				else
					return select_next(current->parent, current);
			}

		public:
			NodeType& operator*() { return m_Current; }
			NodeType operator*() const { return m_Current; }
			NodeType operator->() const { return m_Current; }

			AnyIterator<NodeType>& operator++() {
				m_Current = select_next(m_Current);
				return *this;
			}
			AnyIterator<NodeType> operator++(int) {
				AnyIterator copy = *this;
				m_Current = select_next(m_Current);
				return copy;
			}

			// templates because I'm lazy
			template <typename Other>
			bool operator==(const Other& other) const { return m_Current == other.m_Current; }
			template <typename Other>
			bool operator!=(const Other& other) const { return !(*this == other); }
		};

		class Iterator : public AnyIterator<Node*> {
			friend class RBTree;
			friend class ConstIterator;
		public:
			Iterator(Node* n) : AnyIterator<Node*>(n) {}
			Iterator(const Iterator& other) : AnyIterator<Node*>(other.m_Current) {}
		};

		class ConstIterator : public AnyIterator<const Node*> {
			friend class RBTree;
		public:
			ConstIterator(const Node* n) : AnyIterator<const Node*>(n) {}
			ConstIterator(const ConstIterator& other) : AnyIterator<const Node*>(other.m_Current) {}
			ConstIterator(const Iterator& other) : AnyIterator<const Node*>(other.m_Current) {}
		};

		typedef Node* value_type;
		typedef Iterator iterator;
		typedef ConstIterator const_iterator;
	private:
		Comparator m_Comparator;
		Allocator m_Alloc;
		Node* m_Root;
		size_t m_Size;

		bool is_red(Node* node) const { return node && node->color == RED; }

		inline Node* lookup(const K& key) const {
			Node* node = m_Root;
			while (node) {
				int cmp = m_Comparator(key, node->key);
				if (cmp == 0)
					return node;
				node = node->children[cmp > 0];
			}
			return NULL;
		}

		void clear_tree(Node* root) {
			if (root != NULL)
			{
				root->key.~K();
				root->value.~V();
				clear_tree(root->left);
				clear_tree(root->right);
				m_Alloc.free(root);
			}
		}

		inline void replace_node(Node* old, Node* n) {
			if (!old->parent)
				m_Root = n;
			else
				old->parent->children[old != old->parent->left] = n;

			if (n)
				n->parent = old->parent;
		}

		inline void rotate(Node* root, bool direction = LEFT) {
			Node* new_root = root->children[!direction];
			replace_node(root, new_root);
			root->children[!direction] = new_root->children[direction];
			if (new_root->children[direction])
				new_root->children[direction]->parent = root;
			new_root->children[direction] = root;
			root->parent = new_root;
		}

		inline Node* grandparent(Node* n) const {
			ASSERT(n);
			ASSERT(n->parent);
			return n->parent->parent;
		}

		inline Node* sibling(Node* n) const {
			ASSERT(n);
			ASSERT(n->parent);
			return n->parent->children[n == n->parent->left];
		}

		inline Node* uncle(Node* n) const {
			ASSERT(n);
			return sibling(n->parent);
		}

		// TODO: Optimize insert_rebalance_case* to be iterative and a single function
		void insert_rebalance_case1(Node* n) {
			if (!n->parent)
				n->color = BLACK;
			else
				insert_rebalance_case2(n);
		}

		void insert_rebalance_case2(Node* n) {
			if (n->parent->color == BLACK)
				return; // Tree is still valid.
			else
				insert_rebalance_case3(n);
		}

		void insert_rebalance_case3(Node* n) {
			Node* u = uncle(n);
			if (is_red(u)) {
				n->parent->color = BLACK;
				u->color = BLACK;

				Node* g = grandparent(n);
				g->color = RED;
				insert_rebalance_case1(g);
			} else
				insert_rebalance_case4(n);
		}

		void insert_rebalance_case4(Node* n) {
			Node* g = grandparent(n);
			if (n == n->parent->right && n->parent == g->left) {
				rotate(n->parent, LEFT);
				n = n->left;
			} else if (n == n->parent->left && n->parent == g->right) {
				rotate(n->parent, RIGHT);
				n = n->right;
			}
			insert_rebalance_case5(n);
		}

		void insert_rebalance_case5(Node* n) {
			n->parent->color = BLACK;
			Node* g = grandparent(n);
			g->color = RED;
			if (n == n->parent->left && n->parent == g->left)
				rotate(g, RIGHT);
			else {
				ASSERT(n == n->parent->right && n->parent == g->right);
				rotate(g, LEFT);
			}
		}

		Node* maximum_node(Node* node) {
			ASSERT(node);
			while (node->right) {
				node = node->right;
			}
			return node;
		}

		void delete_rebalance_case1(Node* n) {
			if (!n->parent)
				return;
			else
				delete_rebalance_case2(n);
		}

		void delete_rebalance_case2(Node* n) {
			Node* s = sibling(n);
			if (is_red(s)) {
				n->parent->color = RED;
				s->color = BLACK;
				rotate(n->parent, n != n->parent->left);
			}
			delete_rebalance_case3(n);
		}

		void delete_rebalance_case3(Node* n) {
			Node* s = sibling(n);
			if (!is_red(n->parent) && !is_red(s) && !is_red(s->left) && !is_red(s->right)) {
				s->color = RED;
				delete_rebalance_case1(n->parent);
			} else
				delete_rebalance_case4(n);
		}

		void delete_rebalance_case4(Node* n) {
			Node* s = sibling(n);
			if (is_red(n->parent) && !is_red(s) && !is_red(s->left) && !is_red(s->right)) {
				s->color = RED;
				n->parent->color = BLACK;
			} else
				delete_rebalance_case5(n);
		}

		void delete_rebalance_case5(Node* n) {
			Node* s = sibling(n);
			if (n == n->parent->left && !is_red(s) && is_red(s->left) && !is_red(s->right)) {
				s->color = RED;
				s->left->color = BLACK;
				rotate(s, RIGHT);
			} else if (n == n->parent->right && !is_red(s) && is_red(s->right) && !is_red(s->left)) {
				s->color = RED;
				s->right->color = BLACK;
				rotate(s, LEFT);
			}
			delete_rebalance_case6(n);
		}

		void delete_rebalance_case6(Node* n) {
			Node* s = sibling(n);
			s->color = n->parent->color;
			n->parent->color = BLACK;
			if (n == n->parent->left) {
				ASSERT(is_red(s->right));
				s->right->color = BLACK;
				rotate(n->parent, LEFT);
			} else {
				ASSERT(is_red(s->left));
				s->left->color = BLACK;
				rotate(n->parent, RIGHT);
			}
		}
	public:
		RBTree() : m_Root(NULL), m_Size(0) {}

		~RBTree() {
			clear_tree(m_Root);
		}

		// root is publicly available to make this tree GC-friendly. Use with caution!
		Node*& root() { return m_Root; }

		size_t size() const { return m_Size; }
		void clear() { clear_tree(m_Root); m_Size = 0; }

		Iterator begin() { return Iterator(m_Root); }
		Iterator end() { return Iterator(NULL); }
		ConstIterator begin() const { return ConstIterator(m_Root); }
		ConstIterator end() const { return ConstIterator(NULL); }

		ConstIterator find(const K& key) const {
			return ConstIterator(lookup(key));
		}

		Iterator insert(const K& key, const V& value) {
			// Find the place
			Node** place = &m_Root;
			Node* last_parent = NULL;
			while (*place) {
				int cmp = m_Comparator(key, (*place)->key);
				if (cmp == 0)
					break;
				last_parent = *place;
				place = &last_parent->children[cmp > 0];
			}

			if (!*place) {
				// Insert
				*place = (Node*)m_Alloc.alloc(sizeof(Node));
				
				Node* node = *place;
				node->color = RED;
				node->parent = last_parent;
				node->key = key;
				node->value = value;
				node->left = NULL;
				node->right = NULL;

				insert_rebalance_case1(node);

				++m_Size;
			} else {
				// Update
				Node* node = *place;
				node->value = value;
			}

			return Iterator(*place);
		}

		V& operator[](const K& key) {
			Node* n = lookup(key);
			if (n) {
				return n->value;
			} else {
				Iterator iter = insert(key, V());
				return iter->value;
			}

		}

		// TODO: Iterator-based delete
		bool erase(const K& key, V* erased = NULL) {
			Node* n = lookup(key);
			if (!n) return false;

			if (erased)
				*erased = n->value;

			if (n->left && n->right) {
				// Copy key/value from predecessor and then delete it instead
				// TODO: Get rid of this... We are not interesting in enforcing K to have a public copy constructor
				Node* pred = maximum_node(n->left);
				n->key = pred->key;
				n->value = pred->value;
				n = pred;
			}

			ASSERT(!n->left || !n->right);
			Node* child = n->children[!!n->right];
			if (!is_red(n)) {
				n->color = is_red(child) ? RED : BLACK;
				delete_rebalance_case1(n);
			}
			replace_node(n, child);
			if (!n->parent && child)
				child->color = BLACK;

			for (auto iter = begin(); iter != end(); ++iter) {
				ASSERT(iter->parent != n);
				ASSERT(iter->left != n);
				ASSERT(iter->right != n);
			}

			n->key.~K();
			n->value.~V();
			m_Alloc.free(n);
			--m_Size;

			ASSERT(verify());
			return true;
		}

		size_t height() const {
			return height(m_Root);
		}

		bool verify() const {
			return verify(m_Root);
		}
	private:
		size_t height(Node* root) const {
			if (!root) return 0;
			size_t lh = height(root->left);
			size_t rh = height(root->right);
			return 1 + (lh > rh ? lh : rh);
		}

		bool verify(Node* root) const {
			if (!root)
				return true;

			Node* left = root->left;
			Node* right = root->right;

			if (is_red(root)) {
				if (is_red(left) || is_red(right)) {
					ASSERT(false && "red violation");
					return false;
				}
			}

			if ((left && m_Comparator(left->key, root->key) >= 0) || (right && m_Comparator(right->key, root->key) <= 0)) {
				ASSERT(false && "binary tree violation");
				return false;
			}

			int left_bh = black_height(left);
			int right_bh = black_height(right);

			if (left_bh != 0 && right_bh != 0 && left_bh != right_bh) {
				ASSERT(false && "black violation");
				return false;
			}

			return true;
		}

		int black_height(Node* root) const {
			if (!root)
				return 0;
			int left_bh = black_height(root->left);
			int right_bh = black_height(root->right);
			int self_height = is_red(root) ? 0 : 1;
			int max_child_height = left_bh > right_bh ? left_bh : right_bh;
			return self_height + max_child_height;
		}
	};
}

#endif // RBTREE_H_9JSHAD

