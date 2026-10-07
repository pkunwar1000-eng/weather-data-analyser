#ifndef ICT283_TIME_H
#define ICT283_TIME_H

	/**
	 * @class Time
	 * @brief Stores a time of day as hour and minute (24-hour clock).
	 *
	 * Time is a simple, reusable value class. In this assignment it holds the time
	 * part of each WAST reading from the weather data file, but it has no knowledge
	 * of weather data and can be reused anywhere an hour/minute value is needed.
	 *
	 * The class stores only data and provides plain get/set access. It has no I/O
	 * methods and no friends, so its values cannot be changed from outside except
	 * through its setters.
	 *
	 * @author Prasanna Kunwar
	 * @version 02
	 * @date 15/06/2026 Started Lab 5 Date class
	 * @date 25/06/2026 Re-documented for Assignment 1
	 */
class Time
{
public:

		/**
		 * @brief Creates a Time with hour and minute set to zero.
		 */
	Time();

		/**
		 * @brief Creates a Time with the given hour and minute.
		 *
		 * @param hour - hour value
		 * @param minute - minute value
		 */
	Time(int hour, int minute);

		/**
		 * @brief Sets the hour value.
		 *
		 * @param hour - hour value
		 */
	void SetHour(int hour);

		/**
		 * @brief Sets the minute value.
		 *
		 * @param minute - minute value
		 */
	void SetMinute(int minute);

		/**
		 * @brief Returns the hour value.
		 *
		 * @return int - hour value
		 */
	int GetHour() const;

		/**
		 * @brief Returns the minute value.
		 *
		 * @return int - minute value
		 */
	int GetMinute() const;

private:
		/// Hour value
	int m_hour;

		/// Minute value
	int m_minute;
};

#endif
