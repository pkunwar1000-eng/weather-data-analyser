#ifndef ICT283_WEATHER_DATA_STORE_H
#define ICT283_WEATHER_DATA_STORE_H

#include "Map.h"
#include "BST.h"
#include "Vector.h"
#include "KeyedRecord.h"
#include "WeatherRecord.h"

	/**
	 * @class WeatherDataStore
	 * @brief Holds every weather reading, grouped by the month it was taken in.
	 *
	 * The readings are kept in a map of trees. The map holds one entry for each
	 * month that has readings, labelled with that month's key, such as 201406
	 * for June 2014. Behind each label is a binary search tree holding all the
	 * readings taken during that month.
	 *
	 * Grouping by month was chosen because every menu option asks about either
	 * a month or a year. Asking about one month is a single look-up. Asking
	 * about a whole year is twelve look-ups, and those twelve sit next to each
	 * other because the map keeps its labels in order.
	 *
	 * The readings inside a month are kept in a tree because a tree can find
	 * one reading without looking at all the others, and can hand its readings
	 * back one at a time to whatever wants to work with them. Each month holds
	 * a few thousand readings, which is small enough for a tree to stay quick
	 * and large enough that checking them one by one would not be.
	 *
	 * Inside a tree the readings are keyed by their date and time hashed
	 * together. Every reading has a different date and time, so none is lost as
	 * a repeat, and hashing stops the tree turning into a long line as the
	 * readings arrive in time order. See WeatherKey for why.
	 *
	 * Readings are handed back by giving this class a routine and having it
	 * call that routine once for each reading. This class never prints anything
	 * and never decides what a reading means. Whatever asked for the readings
	 * decides that.
	 *
	 * @author Prasanna Kunwar
	 * @version 01
	 * @date 22/07/2026 Prasanna Kunwar, Created for Assignment 2
	 */
class WeatherDataStore
{
public:

		/**
		 * @brief The type of routine that the reading walks can call back.
		 *
		 * This names a pointer to a routine that takes one reading and returns
		 * nothing. The reading comes through as a const reference, so the
		 * routine can read it but cannot change what is stored.
		 */
	typedef void (*Visit)(const KeyedRecord& reading);

		/**
		 * @brief Makes an empty store, holding no months.
		 *
		 * @return void
		 */
	WeatherDataStore();											// management method

		/**
		 * @brief Puts a set of readings into the store, grouped by month.
		 *
		 * Each month's tree is built completely before it is put into the map,
		 * because taking a tree out of the map and putting it back copies the
		 * whole tree twice, which would be far too slow to do once per reading.
		 *
		 * This may be called more than once. Readings for a month already in
		 * the store are added to it rather than replacing what is there.
		 *
		 * @param readings - the readings to store
		 * @return void
		 */
	void Load(const Vector<WeatherRecord>& readings);			// primary behaviour

		/**
		 * @brief Calls the given routine for every reading in one month.
		 *
		 * If that month has no readings the routine is not called at all.
		 *
		 * @param year - the year wanted
		 * @param month - the month wanted, 1 to 12
		 * @param visit - the routine to call for each reading
		 * @return bool - true if that month was in the store
		 */
	bool ForEachInMonth(int year, int month, Visit visit) const;	// primary behaviour

		/**
		 * @brief Calls the given routine for readings in one month of any year.
		 *
		 * This goes through every month held and uses the ones whose month
		 * matches, whatever year they belong to. It is what menu option 3
		 * needs, because that option asks about a month across all the years
		 * that have been loaded.
		 *
		 * @param month - the month wanted, 1 to 12
		 * @param visit - the routine to call for each reading
		 * @return int - how many years had readings for that month
		 */
	int ForEachInMonthAllYears(int month, Visit visit) const;	// primary behaviour

		/**
		 * @brief Returns how many months have readings in them.
		 *
		 * @return int - the number of months held
		 */
	int MonthCount() const;										// query status

		/**
		 * @brief Returns how many readings are held altogether.
		 *
		 * @return int - the total number of readings across every month
		 */
	int ReadingCount() const;									// query status

private:

		/// The months, each labelled with its month key and holding a tree of
		/// that month's readings.
	Map<int, Bst<KeyedRecord> > m_months;
};

#endif
