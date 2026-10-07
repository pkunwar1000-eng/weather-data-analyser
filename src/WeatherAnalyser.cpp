#include "WeatherAnalyser.h"
#include "Statistics.h"
#include "Vector.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::fixed;
using std::setprecision;

// Everything in this unnamed namespace is private to this file.
namespace
{
		// The store hands its readings over by calling a routine, and a plain
		// routine cannot carry anything with it. So the gathered readings are
		// collected here, where the routines that gather them can reach them.
		// Each list is emptied before the gathering that fills it.
	Vector<float> g_wind;
	Vector<float> g_temp;
	Vector<float> g_solar;

	// Names of the months, used for printing. Index 0 is unused so that
	// month 1 is January and no adjusting is needed when looking a name up.
	const char* MONTH_NAMES[13] =
	{
		"", "January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"
	};

	// Empties all three lists before a new gathering starts.
	void ClearGathered()
	{
		g_wind.Clear();
		g_temp.Clear();
		g_solar.Clear();
	}

	// Gathers the wind speed from a reading, changed into kilometres per hour.
	// The data file holds it in metres per second, and one metre per second is
	// 3.6 kilometres per hour. A reading with no wind value is skipped.
	void GatherWind(const KeyedRecord& reading)
	{
		if(reading.hasWindSpeed)
		{
			g_wind.Add(reading.windSpeed * 3.6);
		}
	}

	// Gathers the temperature from a reading. It is already in degrees Celsius.
	// A reading with no temperature value is skipped.
	void GatherTemperature(const KeyedRecord& reading)
	{
		if(reading.hasTemperature)
		{
			g_temp.Add(reading.temperature);
		}
	}

	// Gathers all three values from a reading, keeping them lined up so that
	// the value at a given position in one list came from the same reading as
	// the value at that position in the others. The correlation needs that,
	// because two values only mean something together if they were measured at
	// the same moment. A reading is only taken when all three values are
	// present, so the three lists stay the same length and a pair is never
	// formed from a real value on one side and a missing value on the other.
	void GatherAllThree(const KeyedRecord& reading)
	{
		if(reading.hasWindSpeed
		   && reading.hasTemperature
		   && reading.hasSolarRadiation)
		{
			g_wind.Add(reading.windSpeed);
			g_temp.Add(reading.temperature);
			g_solar.Add(reading.solarRadiation);
		}
	}

	// Gathers everything needed for one line of the output file. Wind is
	// changed into kilometres per hour. Solar radiation is kept as it is here
	// and added up later, because only some of the readings count towards it.
	// A field that was missing in the data is not added, so a month with no
	// values for that field ends up with an empty list and the report leaves
	// that field blank instead of printing a made-up zero.
	void GatherForFile(const KeyedRecord& reading)
	{
		if(reading.hasWindSpeed)
		{
			g_wind.Add(reading.windSpeed * 3.6);
		}
		if(reading.hasTemperature)
		{
			g_temp.Add(reading.temperature);
		}
		if(reading.hasSolarRadiation)
		{
			g_solar.Add(reading.solarRadiation);
		}
	}

	// Adds up the solar radiation for a month in kilowatt hours per square
	// metre. Readings below 100 watts per square metre are left out, as the
	// assignment requires. Each reading covers ten minutes, which is a sixth of
	// an hour, and a watt hour is a thousandth of a kilowatt hour.
	float TotalSolar(const Vector<float>& solar)
	{
		float total = 0.0;

		for(int i = 0; i < solar.GetSize(); i++)
		{
			if(solar[i] >= 100.0)
			{
				total = total + ((solar[i] / 6.0) / 1000.0);
			}
		}

		return total;
	}
}

void ReportWindForMonth(const WeatherDataStore& store, int year, int month)
{
	ClearGathered();

	store.ForEachInMonth(year, month, GatherWind);

	cout << MONTH_NAMES[month] << " " << year << ":";

	if(g_wind.GetSize() == 0)
	{
		cout << " No Data" << endl;
		return;
	}

	cout << endl;
	cout << fixed << setprecision(1);
	cout << "Average speed: " << Average(g_wind) << " km/h" << endl;
	cout << "Sample stdev: " << SampleStandardDeviation(g_wind) << endl;
}

void ReportTemperatureForYear(const WeatherDataStore& store, int year)
{
	cout << year << endl;

	for(int month = 1; month <= 12; month++)
	{
		ClearGathered();

		store.ForEachInMonth(year, month, GatherTemperature);

		cout << MONTH_NAMES[month] << ":";

		if(g_temp.GetSize() == 0)
		{
			cout << " No Data" << endl;
			continue;
		}

		cout << fixed << setprecision(1);
		cout << " average: " << Average(g_temp) << " degrees C";
		cout << ", stdev: " << SampleStandardDeviation(g_temp) << endl;
	}
}

void ReportCorrelationsForMonth(const WeatherDataStore& store, int month)
{
	ClearGathered();

	// Gather that month from every year that has been loaded. All three values
	// are taken from each reading at once, so they stay lined up.
	store.ForEachInMonthAllYears(month, GatherAllThree);

	cout << "Sample Pearson Correlation Coefficient for " << MONTH_NAMES[month] << endl;

	if(g_wind.GetSize() < 2)
	{
		cout << "No Data" << endl;
		return;
	}

	cout << fixed << setprecision(2);
	cout << "S_T: " << sPCC(g_wind, g_temp) << endl;
	cout << "S_R: " << sPCC(g_wind, g_solar) << endl;
	cout << "T_R: " << sPCC(g_temp, g_solar) << endl;
}

bool HasDataForYear(const WeatherDataStore& store, int year)
{
	for(int month = 1; month <= 12; month++)
	{
		ClearGathered();

			// Gather all fields, because a year counts as having data when any
			// of its fields has readings. Checking only one field would wrongly
			// call the year empty if that single field happened to be the one
			// missing from the data while the others were present.
		store.ForEachInMonth(year, month, GatherForFile);

		if(g_wind.GetSize() > 0
		   || g_temp.GetSize() > 0
		   || g_solar.GetSize() > 0)
		{
			return true;
		}
	}

	return false;
}

bool WriteYearToFile(const WeatherDataStore& store, int year, const string& outputFileName)
{
	ofstream outfile(outputFileName.c_str());

	if(!outfile)
	{
		return false;
	}

	outfile << year << endl;

		// A heading row, so anyone opening the file can tell what each column
		// holds. The fields match the layout the assignment asks for, where the
		// standard deviation and the mean absolute deviation are shown in
		// brackets beside the average they belong to.
	outfile << "Month,"
	        << "Wind Speed (stdev, mad),"
	        << "Temperature (stdev, mad),"
	        << "Solar Radiation" << endl;

		// Whether any month of the year had readings at all.
	bool anyData = false;

	for(int month = 1; month <= 12; month++)
	{
		ClearGathered();

		store.ForEachInMonth(year, month, GatherForFile);

			// A month is only left out when it has no readings for any field
			// at all. If even one field has data, the month is printed and any
			// field that happens to be missing is shown as a blank, so the two
			// commas sit next to each other as the assignment requires.
		if(g_wind.GetSize() == 0
		   && g_temp.GetSize() == 0
		   && g_solar.GetSize() == 0)
		{
			continue;
		}

		anyData = true;

		outfile << fixed << setprecision(1);
		outfile << MONTH_NAMES[month] << ",";

			// Wind: value with its (stdev, mad) if present, otherwise blank.
		if(g_wind.GetSize() > 0)
		{
			outfile << Average(g_wind) << "("
			        << SampleStandardDeviation(g_wind) << ", "
			        << mad(g_wind) << ")";
		}
		outfile << ",";

			// Temperature: same treatment, blank when the month has none.
		if(g_temp.GetSize() > 0)
		{
			outfile << Average(g_temp) << "("
			        << SampleStandardDeviation(g_temp) << ", "
			        << mad(g_temp) << ")";
		}
		outfile << ",";

			// Solar radiation is a total, so a blank is printed when the month
			// has no solar readings rather than a total of zero.
		if(g_solar.GetSize() > 0)
		{
			outfile << TotalSolar(g_solar);
		}
		outfile << endl;
	}

	if(!anyData)
	{
		outfile << "No Data" << endl;
	}

	return true;
}
