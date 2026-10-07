#ifndef ICT283_DATE_H
#define ICT283_DATE_H

#include <iostream>

	/**
	 * @class Date
	 * @brief Stores a calendar date as day, month and year.
	 *
	 * Date is a simple, reusable value class. In this assignment it holds the date
	 * part of each WAST reading from the weather data file, but it has no knowledge
	 * of weather data and can be reused anywhere a day/month/year is needed.
	 *
	 * The class stores only data and provides plain get/set access. It has no I/O
	 * methods and no friends, so its values cannot be changed from outside except
	 * through its setters.
	 *
	 * @author Prasanna Kunwar
	 * @version 03
	 * @date 15/06/2026 Started Lab 5 Date class
	 * @date 25/06/2026 Re-documented for Assignment 1
	 */
class Date
{
public:

		/**
		 * @brief Creates a Date with day, month and year set to zero.
		 */
	Date();

		/**
		 * @brief Creates a Date with the given day, month and year.
		 *
		 * @param day - day value
		 * @param month - month value
		 * @param year - year value
		 */
	Date(int day, int month, int year);

		/**
		 * @brief Sets the day value.
		 *
		 * @param day - day value
		 */
	void SetDay(int day);

		/**
		 * @brief Sets the month value.
		 *
		 * @param month - month value
		 */
	void SetMonth(int month);

		/**
		 * @brief Sets the year value.
		 *
		 * @param year - year value
		 */
	void SetYear(int year);

		/**
		 * @brief Returns the day value.
		 *
		 * @return int - day value
		 */
	int GetDay() const;

		/**
		 * @brief Returns the month value.
		 *
		 * @return int - month value
		 */
	int GetMonth() const;

		/**
		 * @brief Returns the year value.
		 *
		 * @return int - year value
		 */
	int GetYear() const;

		/**
		 * @brief Checks if this date is earlier than another date.
		 *
		 * Compares by year first, then month, then day. This gives dates their
		 * natural calendar order, which is what the BST needs to sort them.
		 *
		 * @param other - the date to compare against
		 * @return bool - true if this date comes before other
		 */
	bool operator<(const Date& other) const;

		/**
		 * @brief Checks if this date is the same as another date.
		 *
		 * @param other - the date to compare against
		 * @return bool - true if the day, month and year all match
		 */
	bool operator==(const Date& other) const;

private:
		/// Day value
	int m_day;

		/// Month value
	int m_month;

		/// Year value
	int m_year;
};

	/**
	 * @brief Prints a date as day/month/year.
	 *
	 * This is a free function, not a member of Date, so that Date itself stays
	 * free of input/output code. The BST uses it to print dates during its
	 * traversals.
	 *
	 * @param output - the output stream to print to
	 * @param date - the date to print
	 * @return ostream& - the same output stream, so prints can be chained
	 */
std::ostream& operator<<(std::ostream& output, const Date& date);

#endif
