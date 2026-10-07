#ifndef ICT283_KEYED_RECORD_H
#define ICT283_KEYED_RECORD_H

#include "Date.h"
#include "WeatherTime.h"
#include "WeatherRecord.h"
#include "WeatherKey.h"

	/**
	 * @struct KeyedRecord
	 * @brief One weather reading together with the key the tree stores it by.
	 *
	 * The readings are kept in a binary search tree, and a tree needs a way to
	 * compare the things it holds so it knows which side to put them on. This
	 * struct is what the tree actually stores: a reading, plus the key that
	 * decides where it goes.
	 *
	 * The key is the hashed date and time from WeatherKey. Every reading has a
	 * different date and time, so no two readings share a key, and the tree
	 * keeps them all instead of treating any of them as repeats.
	 *
	 * The reason for having this struct at all is that WeatherRecord is the
	 * class from Assignment 1 that simply describes a weather reading. A
	 * reading has no reason to know anything about hashing or about how a tree
	 * decides where to put things. Putting the key in here instead leaves
	 * WeatherRecord exactly as it was, and keeps everything to do with keys in
	 * one place.
	 *
	 * The date and time are kept as well as the key. The key has been hashed,
	 * so the original date and time cannot be worked back out from it, and the
	 * menu options still need them to pick out the readings they want.
	 *
	 * A struct is used rather than a class because this is a small bundle of
	 * values that belong together and has no rules of its own to protect.
	 *
	 * @author Prasanna Kunwar
	 * @version 01
	 * @date 22/07/2026 Prasanna Kunwar, Created for Assignment 2
	 */
struct KeyedRecord
{
		/// The hashed date and time. The tree uses this to order the readings.
	long long key;

		/// The date of the reading, kept because the key cannot be undone.
	Date date;

		/// The time of the reading, kept because the key cannot be undone.
	Time time;

		/// Wind speed in metres per second
	float windSpeed;

		/// Ambient air temperature in degrees Celsius
	float temperature;

		/// Solar radiation in watts per square metre
	float solarRadiation;

		/// Whether each measured field held a value, carried over from the
		/// WeatherRecord so a blank field can still be told apart from a zero
		/// after the reading is stored in the tree.
	bool hasWindSpeed;
	bool hasTemperature;
	bool hasSolarRadiation;

		/**
		 * @brief Creates an empty record with everything set to zero.
		 */
	KeyedRecord();

		/**
		 * @brief Creates a record from a weather reading, working out its key.
		 *
		 * @param record - the weather reading to copy the values from
		 */
	explicit KeyedRecord(const WeatherRecord& record);

		/**
		 * @brief Says whether this record belongs before another one.
		 *
		 * Only the key is compared. This is the one thing the tree needs from
		 * whatever it stores, so it is the only comparison provided.
		 *
		 * @param other - the record to compare against
		 * @return bool - true if this record's key is the smaller one
		 */
	bool operator<(const KeyedRecord& other) const;
};

#endif
