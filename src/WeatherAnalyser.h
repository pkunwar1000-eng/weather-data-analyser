#ifndef ICT283_WEATHER_ANALYSER_H
#define ICT283_WEATHER_ANALYSER_H

#include <string>
#include "WeatherDataStore.h"

	/**
	 * @file WeatherAnalyser.h
	 * @brief The routines that answer the four menu options.
	 *
	 * These routines ask the store for the readings they need and work out the
	 * answers from them. The store hands its readings over one at a time by
	 * calling a routine in this file, so nothing here needs to know how the
	 * readings are actually kept.
	 *
	 * The numbers themselves are worked out by the Statistics routines, which
	 * know nothing about weather. These routines only gather up the right
	 * readings, convert the units, and print the result.
	 *
	 * Printing is done here rather than in the store or in the data classes,
	 * because how something is shown to a user is a separate concern from how
	 * it is kept or measured.
	 *
	 * @author Prasanna Kunwar
	 * @version 01
	 * @date 22/07/2026 Prasanna Kunwar, Created for Assignment 2
	 */

	/**
	 * @brief Menu 1: prints the average wind speed and its spread for one month.
	 *
	 * Wind speed is held in metres per second and is printed in kilometres per
	 * hour. If there are no readings for that month, "No Data" is printed.
	 *
	 * @param store - the stored weather readings
	 * @param year - the year asked for
	 * @param month - the month asked for, 1 to 12
	 * @return void
	 */
void ReportWindForMonth(const WeatherDataStore& store, int year, int month);

	/**
	 * @brief Menu 2: prints the average temperature and its spread for each month.
	 *
	 * Every month of the given year is printed in order. Months with no
	 * readings are printed as "No Data".
	 *
	 * @param store - the stored weather readings
	 * @param year - the year asked for
	 * @return void
	 */
void ReportTemperatureForYear(const WeatherDataStore& store, int year);

	/**
	 * @brief Menu 3: prints how strongly pairs of readings move together.
	 *
	 * Three pairs are worked out for the month asked for: wind speed against
	 * temperature, wind speed against solar radiation, and temperature against
	 * solar radiation.
	 *
	 * The month applies to every year that has been loaded, so a month asked
	 * for once gathers that month from all of those years together.
	 *
	 * The two values in a pair must come from the same reading, otherwise the
	 * result would mean nothing. Readings are gathered whole, so the values
	 * stay matched up.
	 *
	 * @param store - the stored weather readings
	 * @param month - the month asked for, 1 to 12
	 * @return void
	 */
void ReportCorrelationsForMonth(const WeatherDataStore& store, int month);

	/**
	 * @brief Says whether any month of a year has readings.
	 *
	 * This lets the caller tell the user that a year has no data before it
	 * offers to write a file, rather than reporting that a file was written
	 * when there was nothing to put in it.
	 *
	 * @param store - the stored weather readings
	 * @param year - the year to check
	 * @return bool - true if at least one month of that year has readings
	 */
bool HasDataForYear(const WeatherDataStore& store, int year);

	/**
	 * @brief Menu 4: writes a year's figures to a comma separated file.
	 *
	 * Each month that has readings gets one line holding the average wind
	 * speed, the average temperature, and the total solar radiation, with the
	 * standard deviation and mean absolute deviation shown beside the two
	 * averages. Months with no readings are left out. A field with no readings
	 * is left blank between its commas.
	 *
	 * @param store - the stored weather readings
	 * @param year - the year asked for
	 * @param outputFileName - the name of the file to write
	 * @return bool - true if the file was written
	 */
bool WriteYearToFile(const WeatherDataStore& store, int year, const std::string& outputFileName);

#endif
