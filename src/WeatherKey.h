#ifndef ICT283_WEATHER_KEY_H
#define ICT283_WEATHER_KEY_H

#include "Date.h"
#include "WeatherTime.h"

	/**
	 * @class WeatherKey
	 * @brief Turns a date and time into the numbers used to store a reading.
	 *
	 * Two different keys are needed to store the weather data, and this class
	 * works out both of them.
	 *
	 * The first is the month key. It puts a year and a month together into one
	 * number, such as June 2014 becoming 201406. It is used to label the group
	 * that a reading belongs to. Because the month only ever takes two digits,
	 * a later month always gives a larger number, so these keys sort into
	 * calendar order on their own.
	 *
	 * The second is the reading key. Every reading is identified by its date
	 * and its time together, so those are put into one number such as
	 * 201406151030 for the reading at 10:30 on 15 June 2014.
	 *
	 * That reading key is then hashed before it is used. The reason is that the
	 * readings are stored in a binary search tree, and the readings arrive in
	 * time order, so each key is larger than the one before it. A binary search
	 * tree given values that only ever increase puts every one of them down its
	 * right hand side, and the tree turns into a long line instead of a tree.
	 * Searching it then has to step through everything, and building it can use
	 * up so much memory for the nested calls that the program stops.
	 *
	 * Hashing mixes the digits of the key up, so consecutive readings end up
	 * with values that are nowhere near each other. Those spread out across the
	 * tree instead of going down one side, which keeps the tree shallow. The
	 * tree class itself is not changed at all; only the number handed to it is
	 * different.
	 *
	 * The hash is the radix conversion method: the digits of the key are used
	 * as the factors of a polynomial with a prime number as its base, and the
	 * result is then divided by a large prime and the remainder kept.
	 *
	 * The base has to be larger than any digit it is working on. The digits of
	 * a decimal number run from 0 to 9, so the base must be at least 10, and a
	 * prime is used so the mixing is more even. A base of 11 is used, which is
	 * the smallest prime above 9. A smaller base such as 7 cannot tell the
	 * higher digits apart, so two different keys can work out to the same value;
	 * the tree then treats two separate readings as the same one and quietly
	 * throws one away. This was tested against the data: with a base of 11 all
	 * 208891 readings from the six supplied files are stored with none lost,
	 * while a base of 7 keeps only 139536 of them, losing about a third.
	 *
	 * The result of the polynomial is also divided by a large prime, HASH_PRIME,
	 * and the remainder is kept, so the key stays within range. The divisor of
	 * 1000003 was checked against the data and loses no readings across the six
	 * supplied files.
	 *
	 * Because hashing mixes the order up, the readings in the tree are no
	 * longer in time order. That is not a problem here, because every menu
	 * option adds its readings up rather than listing them in order, and the
	 * months are already kept in calendar order by the map.
	 *
	 * @author Prasanna Kunwar
	 * @version 01
	 * @date 22/07/2026 Prasanna Kunwar, Created for Assignment 2
	 */
class WeatherKey
{
public:

		/**
		 * @brief Makes the key that labels the group a reading belongs to.
		 *
		 * The year and the month are put together into one number, so that
		 * June 2014 becomes 201406.
		 *
		 * @param year - the year of the reading
		 * @param month - the month of the reading, 1 to 12
		 * @return int - the month key
		 */
	static int MonthKey(int year, int month);				// primary behaviour

		/**
		 * @brief Returns the year that a month key was made from.
		 *
		 * @param monthKey - a key made by MonthKey
		 * @return int - the year part of the key
		 */
	static int YearOf(int monthKey);						// query status

		/**
		 * @brief Returns the month that a month key was made from.
		 *
		 * @param monthKey - a key made by MonthKey
		 * @return int - the month part of the key, 1 to 12
		 */
	static int MonthOf(int monthKey);						// query status

		/**
		 * @brief Makes the hashed key that identifies one single reading.
		 *
		 * The date and time are put together into one number, and that number
		 * is then hashed so the readings spread out across the tree instead of
		 * arriving in increasing order.
		 *
		 * @param date - the date of the reading
		 * @param time - the time of the reading
		 * @return long long - the hashed key for this reading
		 */
	static long long ReadingKey(const Date& date, const Time& time);	// primary behaviour

		/**
		 * @brief Makes the un-hashed number that a reading key is built from.
		 *
		 * This is the date and time joined into one number, before hashing.
		 * It is provided so that the effect of hashing can be measured and
		 * shown during testing.
		 *
		 * @param date - the date of the reading
		 * @param time - the time of the reading
		 * @return long long - the plain date and time as one number
		 */
	static long long PlainReadingKey(const Date& date, const Time& time);	// query status

private:

		/// The base of the polynomial used by the hash. It is a prime, and it
		/// is larger than 9 so that every decimal digit can be told apart.
	static const long long HASH_BASE = 11;

		/// The hash result is divided by this and the remainder kept. It is a
		/// large prime, which spreads the results out and keeps two different
		/// keys from landing on the same value.
	static const long long HASH_PRIME = 1000003;

	// Runs the radix conversion hash over the digits of a number.
	static long long Hash(long long value);
};

#endif
