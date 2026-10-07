#ifndef ICT283_BST_H
#define ICT283_BST_H

	/**
	 * @class Bst
	 * @brief A Binary Search Tree that keeps its items in sorted order.
	 *
	 * Each item is kept in a node. For every node, the items on its left are
	 * smaller and the items on its right are larger. This rule is the class
	 * invariant, and every method must keep it true.
	 *
	 * The tree is a template, so it does not know or care what type it stores.
	 * The only things the type T must supply are operator< (used to decide left
	 * or right) and a copy constructor (used when a node is built and when the
	 * tree is copied). The tree does not print anything itself, so T does not
	 * need operator<<.
	 *
	 * The tree does not know what the data means, so it must not process the
	 * data either. Instead, the traversals take a pointer to a client routine
	 * (a function pointer) and call that routine once for each item. This is a
	 * call-back: the client decides what happens to each item, and the tree
	 * just walks the nodes.
	 *
	 * The call-back is given the item as a const reference. Const is used on
	 * purpose. If the client could change the item in place, it could change
	 * the part used for comparing, and then the item would be sitting in the
	 * wrong place in the tree. The sorted rule would be broken and the tree
	 * would no longer work, with nothing to warn anyone. Const stops that. The
	 * client can still read every item and keep its own copy, which is all a
	 * collector needs to do.
	 *
	 * Bst does no input or output at all, so it does not include <iostream>.
	 * Any printing is done by the client's call-back routine.
	 *
	 * @author Prasanna Kunwar
	 * @version 02
	 * @date 08/07/2026 Prasanna Kunwar, Created for Lab 9: template Binary Search Tree
	 * @date 20/07/2026 Prasanna Kunwar, Lab 10 (d): traversals changed to use function pointers
	 */
template <class T>
class Bst
{
public:

		/**
		 * @brief The type of routine a traversal can call back.
		 *
		 * This names a pointer to any routine that takes one item as a const
		 * reference and returns nothing. The client writes such a routine and
		 * passes its name to a traversal. Written out in full it is
		 * void (*Visit)(const T&).
		 */
	typedef void (*Visit)(const T& item);

		/**
		 * @brief Makes an empty tree.
		 */
	Bst();

		/**
		 * @brief Makes a full (deep) copy of another tree.
		 *
		 * @param other - the tree to copy from
		 */
	Bst(const Bst<T>& other);

		/**
		 * @brief Frees every node when the tree is destroyed.
		 */
	~Bst();

		/**
		 * @brief Copies another tree into this one, as a deep copy.
		 *
		 * The new copy is built first, then the old nodes are freed, so the
		 * tree is still safe to use even if something went wrong while copying.
		 *
		 * @param other - the tree to copy from
		 * @return Bst<T>& - this tree
		 */
	Bst<T>& operator=(const Bst<T>& other);

		/**
		 * @brief Puts an item into its correct sorted place in the tree.
		 *
		 * If the item is already in the tree it is ignored, so there are no
		 * duplicates.
		 *
		 * @param item - the item to insert
		 * @return void
		 */
	void Insert(const T& item);

		/**
		 * @brief Checks if an item is in the tree.
		 *
		 * @param item - the item to look for
		 * @return bool - true if found, false if not
		 */
	bool Search(const T& item) const;

		/**
		 * @brief Deletes every node so the tree becomes empty.
		 * @return void
		 */
	void DeleteTree();

		/**
		 * @brief Visits every item in sorted order (left, node, right).
		 *
		 * The given routine is called once for each item, in sorted order.
		 *
		 * @param visit - the client routine to call for each item
		 * @return void
		 */
	void InOrder(Visit visit) const;

		/**
		 * @brief Visits every item in pre-order (node, left, right).
		 *
		 * @param visit - the client routine to call for each item
		 * @return void
		 */
	void PreOrder(Visit visit) const;

		/**
		 * @brief Visits every item in post-order (left, right, node).
		 *
		 * @param visit - the client routine to call for each item
		 * @return void
		 */
	void PostOrder(Visit visit) const;

		/**
		 * @brief Checks if the tree has no items.
		 *
		 * @return bool - true if the tree is empty
		 */
	bool IsEmpty() const;

		/**
		 * @brief Returns how many items are in the tree.
		 *
		 * @return int - the number of items
		 */
	int Size() const;

		/**
		 * @brief Checks that the sorted rule still holds everywhere in the tree.
		 *
		 * This is a debug check. It makes sure that for every node the left
		 * items are smaller and the right items are larger. It is used while
		 * testing to catch any method that breaks the tree.
		 *
		 * @return bool - true if the tree is still a valid BST
		 */
	bool IsValid() const;

private:

		/**
		 * @struct Node
		 * @brief One item in the tree, with links to its left and right children.
		 *
		 * Node is private and hidden inside Bst. The client never sees it and
		 * only ever works with items of type T. It is a template because it
		 * holds a T. A struct is used because a Node is only a small bundle of
		 * three values with no behaviour of its own, and it is already hidden
		 * by being private, so making the members private again would only add
		 * getters and setters that nothing outside Bst could ever call.
		 */
	struct Node
	{
			/// The item stored in this node
		T item;
			/// Left child (holds smaller items)
		Node* left;
			/// Right child (holds larger items)
		Node* right;

			/**
			 * @brief Makes a new node that holds one item and has no children yet.
			 * @param value - the item to store
			 */
		explicit Node(const T& value)
			: item(value), left(nullptr), right(nullptr)
		{
		}
	};

		/// The top node of the tree. It is nullptr when the tree is empty.
	Node* m_root;

	// The helpers below do the actual recursive work. They are private, so they
	// use normal code comments, not doxygen comments.

	// Insert item somewhere below node (recursively).
	void InsertHelper(Node*& node, const T& item);

	// Look for item below node (recursively).
	bool SearchHelper(Node* node, const T& item) const;

	// Delete node and everything below it (recursively).
	void DestroyHelper(Node* node);

	// Make a deep copy of node and everything below it (recursively).
	Node* CopyHelper(Node* node) const;

	// Count the nodes below node, including node itself (recursively).
	int SizeHelper(Node* node) const;

	// The three traversals. Each one calls visit for the items below node.
	void InOrderHelper(Node* node, Visit visit) const;
	void PreOrderHelper(Node* node, Visit visit) const;
	void PostOrderHelper(Node* node, Visit visit) const;

	// Check the sorted rule for node and everything below it.
	// Every item must sit between min and max. The two bool flags say whether a
	// min or max bound is set yet, so the root can start with no bounds.
	bool IsValidHelper(Node* node, const T& min, bool hasMin, const T& max, bool hasMax) const;
};

// ---------------------------------------------------------------------------
// Implementation.
//
// The rest of the file is the implementation. Because Bst is a template, the
// implementation has to stay in this header file (there is no Bst.cpp). The
// class interface above is still kept separate from the code down here.
// ---------------------------------------------------------------------------

template <class T>
Bst<T>::Bst()
	: m_root(nullptr)
{
}

template <class T>
Bst<T>::Bst(const Bst<T>& other)
	: m_root(nullptr)
{
	m_root = CopyHelper(other.m_root);
}

template <class T>
Bst<T>::~Bst()
{
	DestroyHelper(m_root);
	m_root = nullptr;
}

template <class T>
Bst<T>& Bst<T>::operator=(const Bst<T>& other)
{
	if(this != &other)
	{
		// Build the new copy first, then delete the old nodes. This way the
		// tree is still fine even if the copy step failed.
		Node* newRoot = CopyHelper(other.m_root);

		DestroyHelper(m_root);
		m_root = newRoot;
	}

	return *this;
}

template <class T>
void Bst<T>::Insert(const T& item)
{
	InsertHelper(m_root, item);
}

template <class T>
void Bst<T>::InsertHelper(Node*& node, const T& item)
{
	// Found an empty spot, so put the new node here. node is passed by
	// reference, so setting it also links it to its parent.
	if(node == nullptr)
	{
		node = new Node(item);
		return;
	}

	if(item < node->item)
	{
		// Smaller, so go left.
		InsertHelper(node->left, item);
	}
	else if(node->item < item)
	{
		// Larger, so go right.
		InsertHelper(node->right, item);
	}
	// Not smaller and not larger means it is already here, so do nothing.
}

template <class T>
bool Bst<T>::Search(const T& item) const
{
	return SearchHelper(m_root, item);
}

template <class T>
bool Bst<T>::SearchHelper(Node* node, const T& item) const
{
	// Ran off the end of a branch without finding it.
	if(node == nullptr)
	{
		return false;
	}

	if(item < node->item)
	{
		// Smaller, so it can only be on the left.
		return SearchHelper(node->left, item);
	}

	if(node->item < item)
	{
		// Larger, so it can only be on the right.
		return SearchHelper(node->right, item);
	}

	// Not smaller and not larger, so this is it.
	return true;
}

template <class T>
void Bst<T>::DeleteTree()
{
	DestroyHelper(m_root);
	m_root = nullptr;
}

template <class T>
void Bst<T>::DestroyHelper(Node* node)
{
	// Delete both children before the node itself, so we never follow a link
	// to a node that is already gone.
	if(node != nullptr)
	{
		DestroyHelper(node->left);
		DestroyHelper(node->right);
		delete node;
	}
}

template <class T>
typename Bst<T>::Node* Bst<T>::CopyHelper(Node* node) const
{
	// Nothing to copy on an empty branch.
	if(node == nullptr)
	{
		return nullptr;
	}

	// Copy this node, then copy its left and right sides.
	Node* copy = new Node(node->item);
	copy->left = CopyHelper(node->left);
	copy->right = CopyHelper(node->right);

	return copy;
}

template <class T>
void Bst<T>::InOrder(Visit visit) const
{
	// If the client passed nothing there is no routine to call, so do nothing
	// rather than following a null pointer and crashing.
	if(visit != nullptr)
	{
		InOrderHelper(m_root, visit);
	}
}

template <class T>
void Bst<T>::InOrderHelper(Node* node, Visit visit) const
{
	// Left, then this item, then right. This visits the items in sorted order.
	if(node != nullptr)
	{
		InOrderHelper(node->left, visit);
		visit(node->item);          // the call-back into the client's routine
		InOrderHelper(node->right, visit);
	}
}

template <class T>
void Bst<T>::PreOrder(Visit visit) const
{
	if(visit != nullptr)
	{
		PreOrderHelper(m_root, visit);
	}
}

template <class T>
void Bst<T>::PreOrderHelper(Node* node, Visit visit) const
{
	// This item first, then left, then right.
	if(node != nullptr)
	{
		visit(node->item);
		PreOrderHelper(node->left, visit);
		PreOrderHelper(node->right, visit);
	}
}

template <class T>
void Bst<T>::PostOrder(Visit visit) const
{
	if(visit != nullptr)
	{
		PostOrderHelper(m_root, visit);
	}
}

template <class T>
void Bst<T>::PostOrderHelper(Node* node, Visit visit) const
{
	// Left, then right, then this item last.
	if(node != nullptr)
	{
		PostOrderHelper(node->left, visit);
		PostOrderHelper(node->right, visit);
		visit(node->item);
	}
}

template <class T>
bool Bst<T>::IsEmpty() const
{
	return m_root == nullptr;
}

template <class T>
int Bst<T>::Size() const
{
	return SizeHelper(m_root);
}

template <class T>
int Bst<T>::SizeHelper(Node* node) const
{
	// An empty branch holds nothing.
	if(node == nullptr)
	{
		return 0;
	}

	// This node, plus everything on both sides.
	return 1 + SizeHelper(node->left) + SizeHelper(node->right);
}

template <class T>
bool Bst<T>::IsValid() const
{
	// Start with no min and no max bound. They get set as we go down.
	T dummy = T();
	return IsValidHelper(m_root, dummy, false, dummy, false);
}

template <class T>
bool Bst<T>::IsValidHelper(Node* node, const T& min, bool hasMin, const T& max, bool hasMax) const
{
	// An empty branch is always fine.
	if(node == nullptr)
	{
		return true;
	}

	// If there is a lower bound, this item must be bigger than it.
	if(hasMin && !(min < node->item))
	{
		return false;
	}

	// If there is an upper bound, this item must be smaller than it.
	if(hasMax && !(node->item < max))
	{
		return false;
	}

	// Going left, this item becomes the new max. Going right, it becomes the
	// new min. Both sides must pass.
	return IsValidHelper(node->left, min, hasMin, node->item, true) &&
	       IsValidHelper(node->right, node->item, true, max, hasMax);
}

#endif
