#ifndef ICT283_WEATHER_RECORD_H
#define ICT283_WEATHER_RECORD_H

#include "Date.h"
#include "WeatherTime.h"

	/**
	 * @struct WeatherRecord
	 * @brief Holds the weather values for a single 10-minute reading.
	 *
	 * WeatherRecord is a passive data bundle: it groups together the date, time
	 * and the three measured values used by this assignment. It is declared as a
	 * struct rather than a class because it has no behaviour and no internal rules
	 * to protect; it simply carries data. Routines that operate on these records
	 * (reading, calculating) are kept outside the struct.
	 *
	 * Units are stored exactly as they appear in the data file:
	 * windSpeed in metres per second, temperature in degrees Celsius and
	 * solarRadiation in watts per square metre. Any unit conversion is done by
	 * the routines that present the output, not here.
	 *
	 * @author Prasanna Kunwar
	 * @version 02
	 * @date 23/06/2026 Created for Assignment 1
	 */
struct WeatherRecord
{
		/// Date of the reading (from the WAST column)
	Date date;

		/// Time of the reading (from the WAST column)
	Time time;

		/// Wind speed in metres per second (column S)
	float windSpeed;

		/// Ambient air temperature in degrees Celsius (column T)
	float temperature;

		/// Solar radiation in watts per square metre (column SR)
	float solarRadiation;

		/// Whether each measured field actually held a value in the data file.
		/// These default to true so that a record built the ordinary way is
		/// treated as complete, which keeps every earlier use unchanged. The
		/// reader sets one to false when that column was blank for a reading,
		/// so a missing field can be told apart from a real value of zero.
	bool hasWindSpeed;
	bool hasTemperature;
	bool hasSolarRadiation;

		/**
		 * @brief Creates a record with the measured values set to zero.
		 */
	WeatherRecord()
		: date(), time(), windSpeed(0.0), temperature(0.0), solarRadiation(0.0),
		  hasWindSpeed(true), hasTemperature(true), hasSolarRadiation(true)
	{
	}
};

#endif
