#include <iostream>
#include "BST.h"
#include "Vector.h"

using std::cout;
using std::endl;

// BST unit-test driver.
//
// This is a SEPARATE program from the main weather application. Its only job is
// to test the Bst class on its own and confirm that every public method works.
// It uses the real BST.h from the common folder, not a copy.
//
// The tree hands its items back through a function pointer, so the checks that
// walk the tree use small call-back routines and file-scope counters. A call-back
// cannot carry anything with it, so what it gathers is kept in these values.

namespace
{
	int g_visitCount = 0;
	int g_sum = 0;
	int g_last = 0;
	bool g_inOrder = true;

	// Counts each item, adds it up, and checks the items arrive in increasing
	// order, which an in-order walk of a BST is supposed to give.
	void Tally(const int& item)
	{
		if(g_visitCount > 0 && item < g_last)
		{
			g_inOrder = false;
		}

		g_last = item;
		g_visitCount++;
		g_sum = g_sum + item;
	}

	void ResetTally()
	{
		g_visitCount = 0;
		g_sum = 0;
		g_last = 0;
		g_inOrder = true;
	}

	void Check(const char* what, bool passed)
	{
		cout << (passed ? "  PASS  " : "  FAIL  ") << what << endl;
	}

	// A call-back that prints each item, so a person can actually see the
	// contents a traversal hands back, not just a PASS/FAIL line. Its signature
	// matches Bst<int>::Visit, which is void (*)(const int&).
	void Print(const int& item)
	{
		cout << item << " ";
	}

	// Prints a tree's items in sorted (in-order) form on one labelled line, so
	// the copy/original comparisons are visible on screen.
	void ShowInOrder(const char* label, const Bst<int>& tree)
	{
		cout << "        " << label << ": ";
		tree.InOrder(Print);
		cout << endl;
	}
}

	// A new tree is empty.
static void TestEmpty()
{
	cout << "--- Empty tree ---" << endl;

	Bst<int> tree;

	Check("IsEmpty is true on a new tree", tree.IsEmpty());
	Check("Size is 0 on a new tree", tree.Size() == 0);
	Check("IsValid is true on an empty tree", tree.IsValid());
}

	// Insert adds items, Size counts them, and a duplicate is ignored.
static void TestInsertAndSize()
{
	cout << "--- Insert and Size ---" << endl;

	Bst<int> tree;
	tree.Insert(50);
	tree.Insert(30);
	tree.Insert(70);
	tree.Insert(20);

	Check("not empty after inserts", !tree.IsEmpty());
	Check("Size is 4", tree.Size() == 4);

		// Inserting a value already in the tree must not add a second node.
	tree.Insert(50);
	Check("Size still 4 after duplicate insert", tree.Size() == 4);
}

	// Search finds what is there and does not find what is not.
static void TestSearch()
{
	cout << "--- Search ---" << endl;

	Bst<int> tree;
	tree.Insert(50);
	tree.Insert(30);
	tree.Insert(70);

	Check("finds 30", tree.Search(30));
	Check("finds 70", tree.Search(70));
	Check("does not find 999", !tree.Search(999));
}

	// An in-order walk returns the items smallest to largest.
static void TestInOrderSorted()
{
	cout << "--- In-order walk is sorted ---" << endl;

	Bst<int> tree;
	tree.Insert(50);
	tree.Insert(30);
	tree.Insert(70);
	tree.Insert(10);
	tree.Insert(40);

	ResetTally();
	tree.InOrder(Tally);

	Check("visited all 5 items", g_visitCount == 5);
	Check("items summed to 200", g_sum == 200);
	Check("items came out in increasing order", g_inOrder);

		// Show the actual contents so the sorted order is visible, not just
		// asserted. Inserted as 50 30 70 10 40; in-order should read 10 40 50 70.
	cout << "        in-order : ";
	tree.InOrder(Print);
	cout << endl;
}

	// The three walks all visit every node, and each is printed so the
	// different visiting orders can be seen on screen.
static void TestAllThreeWalks()
{
	cout << "--- All three walks visit every node ---" << endl;

	Bst<int> tree;
	tree.Insert(50);
	tree.Insert(30);
	tree.Insert(70);

	ResetTally();
	tree.PreOrder(Tally);
	Check("pre-order visited 3", g_visitCount == 3);

	ResetTally();
	tree.PostOrder(Tally);
	Check("post-order visited 3", g_visitCount == 3);

		// Print all three walks of the same small tree (root 50, left 30,
		// right 70) so the ordering of each is visible:
		// in-order 30 50 70, pre-order 50 30 70, post-order 30 70 50.
	cout << "        in-order   : "; tree.InOrder(Print);   cout << endl;
	cout << "        pre-order  : "; tree.PreOrder(Print);  cout << endl;
	cout << "        post-order : "; tree.PostOrder(Print); cout << endl;
}

	// A null call-back must be ignored.
static void TestNullCallback()
{
	cout << "--- Null call-back ---" << endl;

	Bst<int> tree;
	tree.Insert(1);

	tree.InOrder(nullptr);

	Check("null call-back did not crash", true);
}

	// The copy constructor makes a deep copy: deleting the copy must leave the
	// original whole.
static void TestCopyIsDeep()
{
	cout << "--- Copy constructor is deep ---" << endl;

	Bst<int> original;
	original.Insert(50);
	original.Insert(30);
	original.Insert(70);

	Bst<int> copy(original);
	Check("copy has same size", copy.Size() == 3);

		// Show both trees hold the same items right after copying.
	ShowInOrder("original    ", original);
	ShowInOrder("copy        ", copy);

	copy.DeleteTree();
	Check("copy is now empty", copy.IsEmpty());
	Check("original still has 3 nodes", original.Size() == 3);

		// Show the copy is now empty while the original is untouched: this is
		// what "deep copy" means, made visible.
	ShowInOrder("copy cleared ", copy);
	ShowInOrder("original kept", original);
}

	// The assignment operator also makes a deep copy.
static void TestAssignmentIsDeep()
{
	cout << "--- Assignment is deep ---" << endl;

	Bst<int> original;
	original.Insert(1);
	original.Insert(2);

	Bst<int> other;
	other.Insert(99);

	other = original;
	Check("assigned tree has 2 nodes", other.Size() == 2);

		// Show both trees match right after assignment (other started as 99).
	ShowInOrder("original     ", original);
	ShowInOrder("after other= ", other);

	other.DeleteTree();
	Check("original unchanged after clearing the assigned tree", original.Size() == 2);

		// Show the assigned tree is empty while the original still holds 1 2.
	ShowInOrder("other cleared", other);
	ShowInOrder("original kept", original);
}

	// DeleteTree empties the tree.
static void TestDeleteTree()
{
	cout << "--- DeleteTree ---" << endl;

	Bst<int> tree;
	tree.Insert(1);
	tree.Insert(2);
	tree.Insert(3);

	tree.DeleteTree();

	Check("empty after DeleteTree", tree.IsEmpty());
	Check("Size 0 after DeleteTree", tree.Size() == 0);
}

int main()
{
	cout << "=========== BST unit tests ===========" << endl;

	TestEmpty();
	TestInsertAndSize();
	TestSearch();
	TestInOrderSorted();
	TestAllThreeWalks();
	TestNullCallback();
	TestCopyIsDeep();
	TestAssignmentIsDeep();
	TestDeleteTree();

	cout << "=========== BST tests done ===========" << endl;

	return 0;
}
