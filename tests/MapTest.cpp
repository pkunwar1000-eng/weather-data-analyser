#include <iostream>
#include "Map.h"
#include "Vector.h"

using std::cout;
using std::endl;

// Map unit-test driver.
//
// This is a SEPARATE program from the main weather application. Its only job is
// to test the Map class on its own and confirm that every public method behaves
// as the interface promises. It uses the real Map.h from the common folder, not
// a copy, so it tests exactly the code the application uses.
//
// Map hands its pairs back through a function pointer, so the checks that walk
// the whole Map use small call-back routines and file-scope counters, the same
// way the application does. A call-back cannot carry anything with it, so the
// results it gathers are kept in these file-scope values.

namespace
{
	int g_visitCount = 0;
	int g_keySum = 0;
	int g_valueSum = 0;
	int g_lastKey = 0;
	bool g_keysInOrder = true;

	// Counts each pair and adds up keys and values. Also checks the keys arrive
	// in increasing order, which a Map is supposed to guarantee.
	void Tally(const int& key, const int& value)
	{
		if(g_visitCount > 0 && key < g_lastKey)
		{
			g_keysInOrder = false;
		}

		g_lastKey = key;
		g_visitCount++;
		g_keySum = g_keySum + key;
		g_valueSum = g_valueSum + value;
	}

	// Resets the counters before a walk.
	void ResetTally()
	{
		g_visitCount = 0;
		g_keySum = 0;
		g_valueSum = 0;
		g_lastKey = 0;
		g_keysInOrder = true;
	}

	// Prints a pass or fail line for one check.
	void Check(const char* what, bool passed)
	{
		cout << (passed ? "  PASS  " : "  FAIL  ") << what << endl;
	}

	// A call-back that prints each key->value pair, so a person can see the
	// pairs ForEach hands back and the key order, not just a PASS/FAIL line.
	// Its signature matches Map<int,int>::Visit, void (*)(const int&, const int&).
	void Print(const int& key, const int& value)
	{
		cout << key << "->" << value << "  ";
	}
}

	// A fresh Map holds nothing.
static void TestEmpty()
{
	cout << "--- Empty Map ---" << endl;

	Map<int, int> m;

	Check("Size is 0 on a new Map", m.Size() == 0);
}

	// Insert adds pairs, Size counts them, Find gets them back.
static void TestInsertAndFind()
{
	cout << "--- Insert and Find ---" << endl;

	Map<int, int> m;
	m.Insert(3, 30);
	m.Insert(1, 10);
	m.Insert(2, 20);

	Check("Size is 3 after three inserts", m.Size() == 3);

	int value = 0;
	Check("Find(1) returns true", m.Find(1, value));
	Check("Find(1) gives 10", value == 10);

	Check("Find(2) returns true", m.Find(2, value));
	Check("Find(2) gives 20", value == 20);

	Check("Find(3) returns true", m.Find(3, value));
	Check("Find(3) gives 30", value == 30);
}

	// Find on a key that is not there returns false and leaves the value alone.
static void TestFindMissing()
{
	cout << "--- Find a missing key ---" << endl;

	Map<int, int> m;
	m.Insert(1, 10);

	int value = 999;
	Check("Find(50) returns false", !m.Find(50, value));
	Check("value left unchanged when key missing", value == 999);
}

	// Inserting a key that is already there replaces the value, and does not add
	// a second pair.
static void TestDuplicateKey()
{
	cout << "--- Duplicate key replaces ---" << endl;

	Map<int, int> m;
	m.Insert(1, 10);
	m.Insert(1, 99);

	Check("Size still 1 after re-inserting same key", m.Size() == 1);

	int value = 0;
	m.Find(1, value);
	Check("value replaced with 99", value == 99);
}

	// ForEach visits every pair exactly once, in key order.
static void TestForEach()
{
	cout << "--- ForEach walks every pair in key order ---" << endl;

	Map<int, int> m;
	m.Insert(30, 1);
	m.Insert(10, 2);
	m.Insert(20, 3);

	ResetTally();
	m.ForEach(Tally);

	Check("visited all 3 pairs", g_visitCount == 3);
	Check("keys summed to 60", g_keySum == 60);
	Check("values summed to 6", g_valueSum == 6);
	Check("keys came out in increasing order", g_keysInOrder);

		// Show the actual pairs so the key order is visible, not just asserted.
		// Inserted as 30 10 20; ForEach should print them as 10 20 30.
	cout << "        pairs : ";
	m.ForEach(Print);
	cout << endl;
}

	// A null call-back must be ignored, not followed into a crash.
static void TestForEachNull()
{
	cout << "--- ForEach with a null call-back ---" << endl;

	Map<int, int> m;
	m.Insert(1, 10);

	m.ForEach(nullptr);

	Check("null call-back did not crash", true);
}

	// Map must work with a value type that is itself a class, not just an int,
	// because the application stores whole trees as values.
static void TestClassValue()
{
	cout << "--- Map holding a Vector as its value ---" << endl;

	Map<int, Vector<int> > m;

	Vector<int> list;
	list.Add(5);
	list.Add(6);
	m.Insert(100, list);

	Vector<int> got;
	Check("Find returns the stored Vector", m.Find(100, got));
	Check("stored Vector has 2 items", got.GetSize() == 2);
	Check("first item is 5", got[0] == 5);

		// Changing the copy must not change what is stored.
	got.Add(7);
	Vector<int> again;
	m.Find(100, again);
	Check("stored Vector unchanged by editing the copy", again.GetSize() == 2);
}

int main()
{
	cout << "=========== Map unit tests ===========" << endl;

	TestEmpty();
	TestInsertAndFind();
	TestFindMissing();
	TestDuplicateKey();
	TestForEach();
	TestForEachNull();
	TestClassValue();

	cout << "=========== Map tests done ===========" << endl;

	return 0;
}
