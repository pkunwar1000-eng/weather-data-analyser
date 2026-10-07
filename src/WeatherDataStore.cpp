#include "WeatherDataStore.h"
#include "WeatherKey.h"

// Everything in this unnamed namespace is private to this file. It is not part
// of the class and nothing outside this file can reach it.
namespace
{
		// A call-back is a plain function pointer, so it cannot carry anything
		// along with it. These file-scope values are how the routines below are
		// told what to look for and where to send what they find. Each is set
		// immediately before the walk that uses it.
	int g_wantedMonth = 0;
	WeatherDataStore::Visit g_forward = nullptr;
	int g_yearsMatched = 0;
	int g_total = 0;

	// Works out which month a reading belongs to.
	int MonthKeyOf(const WeatherRecord& reading)
	{
		return WeatherKey::MonthKey(reading.date.GetYear(), reading.date.GetMonth());
	}

	// Says whether a value is already in a Vector of ints.
	bool IsInVector(const Vector<int>& values, int wanted)
	{
		for(int i = 0; i < values.GetSize(); i++)
		{
			if(values[i] == wanted)
			{
				return true;
			}
		}

		return false;
	}

	// Passes one reading on to whatever the caller asked for.
	void ForwardReading(const KeyedRecord& reading)
	{
		if(g_forward != nullptr)
		{
			g_forward(reading);
		}
	}

	// Used when walking the map. Looks at one month, and if its month part
	// matches the one being looked for, hands on all of that month's readings.
	void MonthIfMatching(const int& monthKey, const Bst<KeyedRecord>& readings)
	{
		if(WeatherKey::MonthOf(monthKey) != g_wantedMonth)
		{
			return;
		}

		g_yearsMatched++;

		readings.InOrder(ForwardReading);
	}

	// Used when walking the map. Adds up how many readings each month holds.
	// The month key is not needed here, because every month is counted, but the
	// call-back has to take one to match what the map hands over.
	void AddUpReadings(const int&, const Bst<KeyedRecord>& readings)
	{
		g_total = g_total + readings.Size();
	}
}

WeatherDataStore::WeatherDataStore()
	: m_months()
{
}

void WeatherDataStore::Load(const Vector<WeatherRecord>& readings)
{
	// The readings are put into the map one month at a time.
	//
	// The obvious way would be to take a month's tree out of the map, add one
	// reading to it, and put it back, once for every reading. But taking a tree
	// out of the map makes a copy of the whole tree, and putting it back copies
	// it again. Doing that for every one of a few hundred thousand readings
	// would copy millions of nodes and take far too long.
	//
	// So the months are worked out first, and then each month's tree is built
	// completely and put into the map once.

		// Step 1: work out which months the readings cover.
	Vector<int> monthKeys;

	for(int i = 0; i < readings.GetSize(); i++)
	{
		int monthKey = MonthKeyOf(readings[i]);

		if(!IsInVector(monthKeys, monthKey))
		{
			monthKeys.Add(monthKey);
		}
	}

		// Step 2: build one tree per month and put it into the map.
	for(int m = 0; m < monthKeys.GetSize(); m++)
	{
		int monthKey = monthKeys[m];

			// Start from whatever this month already holds, so that calling
			// Load a second time adds to the month instead of replacing it.
		Bst<KeyedRecord> tree;
		m_months.Find(monthKey, tree);

		for(int i = 0; i < readings.GetSize(); i++)
		{
			if(MonthKeyOf(readings[i]) == monthKey)
			{
				tree.Insert(KeyedRecord(readings[i]));
			}
		}

		m_months.Insert(monthKey, tree);
	}
}

bool WeatherDataStore::ForEachInMonth(int year, int month, Visit visit) const
{
	if(visit == nullptr)
	{
		return false;
	}

	Bst<KeyedRecord> readings;

	if(!m_months.Find(WeatherKey::MonthKey(year, month), readings))
	{
		return false;
	}

	// Hand every reading in that month to the caller's routine.
	readings.InOrder(visit);

	return true;
}

int WeatherDataStore::ForEachInMonthAllYears(int month, Visit visit) const
{
	if(visit == nullptr)
	{
		return 0;
	}

	// Tell the file-scope routine what to look for and where to send it, then
	// walk every month held.
	g_wantedMonth = month;
	g_forward = visit;
	g_yearsMatched = 0;

	m_months.ForEach(MonthIfMatching);

	g_forward = nullptr;

	return g_yearsMatched;
}

int WeatherDataStore::MonthCount() const
{
	return m_months.Size();
}

int WeatherDataStore::ReadingCount() const
{
	g_total = 0;

	m_months.ForEach(AddUpReadings);

	return g_total;
}
