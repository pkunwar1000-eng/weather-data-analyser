#include <iostream>
#include <string>
#include "Vector.h"
#include "WeatherRecord.h"
#include "WeatherDataReader.h"
#include "WeatherDataStore.h"
#include "WeatherAnalyser.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

// High Level Algorithm:
//   Input:      the data file names listed inside data/data_source.txt, and the
//               menu choices and month/year values typed by the user.
//   Processing: every named file is read once into a list of readings, and that
//               list is then put into the store, which groups the readings by
//               month. Each menu choice asks the store for the readings it
//               needs and works out the answer from them.
//   Output:     menu items 1 to 3 print to the screen, menu item 4 writes the
//               file WindTempSolar.csv.

	// Fixed locations. Only data_source.txt is named in the program itself.
	// The names of the actual data files are read from it, so the program never
	// has to ask the user for a file name and new files can be added by editing
	// that one file.
const string DATA_SOURCE_PATH = "data/data_source.txt";
const string DATA_FOLDER = "data/";
const string OUTPUT_FILE_NAME = "WindTempSolar.csv";

void DisplayMenu();
int ReadIntInRange(const string& prompt, int low, int high);
int ReadYear();
int ReadMonth();
bool LoadAllFiles(WeatherDataStore& store);
void DoWindReport(const WeatherDataStore& store);
void DoTemperatureReport(const WeatherDataStore& store);
void DoCorrelationReport(const WeatherDataStore& store);
void DoFileOutput(const WeatherDataStore& store);

int main()
{
	WeatherDataStore store;

	if(!LoadAllFiles(store))
	{
		return -1;
	}

	cout << "Loaded " << store.ReadingCount() << " weather readings across "
	     << store.MonthCount() << " months." << endl;

	int choice = 0;

	do
	{
		DisplayMenu();
		cin >> choice;

			// Guard against something that is not a number being typed in,
			// otherwise the read fails, nothing is taken off the input, and the
			// loop spins forever on the same bad text.
		if(cin.fail())
		{
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Invalid input. Please enter a number between 1 and 5." << endl;
			choice = 0;
			continue;
		}

		cout << endl;

		switch(choice)
		{
		case 1:
			DoWindReport(store);
			break;

		case 2:
			DoTemperatureReport(store);
			break;

		case 3:
			DoCorrelationReport(store);
			break;

		case 4:
			DoFileOutput(store);
			break;

		case 5:
			cout << "Exiting program." << endl;
			break;

		default:
			cout << "Invalid choice. Please enter a number between 1 and 5." << endl;
		}

	} while(choice != 5);

	return 0;
}

	// Reads every data file named in data_source.txt and puts all the readings
	// into the store. Returns false only if nothing at all could be loaded,
	// because a program with no data cannot answer anything.
bool LoadAllFiles(WeatherDataStore& store)
{
	Vector<string> fileNames;
	Vector<WeatherRecord> readings;

	if(!ReadDataSourceNames(DATA_SOURCE_PATH, fileNames))
	{
		cout << "Could not read " << DATA_SOURCE_PATH << endl;
		return false;
	}

	for(int i = 0; i < fileNames.GetSize(); i++)
	{
		string path = DATA_FOLDER + fileNames[i];

		int before = readings.GetSize();

		if(!LoadWeatherData(path, readings))
		{
			// One unreadable file is not worth stopping for. Say so and carry
			// on with the rest.
			cout << "Warning: could not load " << path << endl;
		}
		else
		{
			cout << "  " << fileNames[i] << ": "
			     << (readings.GetSize() - before) << " readings" << endl;
		}
	}

	if(readings.GetSize() == 0)
	{
		cout << "No weather readings were loaded." << endl;
		return false;
	}

	store.Load(readings);

	return true;
}

void DisplayMenu()
{
	cout << endl;
	cout << "---------- Weather Data Menu ----------" << endl;
	cout << "1. Average wind speed for a month and year" << endl;
	cout << "2. Average temperature for each month of a year" << endl;
	cout << "3. Correlations (sPCC) for a month across all years" << endl;
	cout << "4. Write wind, temperature and solar to a file for a year" << endl;
	cout << "5. Exit" << endl;
	cout << "Enter your choice (1-5): ";
}

int ReadIntInRange(const string& prompt, int low, int high)
{
	int value = 0;
	bool haveValue = false;

	while(!haveValue)
	{
		cout << prompt;
		cin >> value;

		if(cin.fail())
		{
			// Something that is not a number was typed. Clear the failure and
			// throw away the rest of the line before asking again.
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Please enter a whole number." << endl;
		}
		else if(value < low || value > high)
		{
			cout << "Please enter a value between " << low << " and " << high << "." << endl;
		}
		else
		{
			haveValue = true;
		}
	}

	return value;
}

int ReadYear()
{
	return ReadIntInRange("Enter year (e.g. 2014): ", 1900, 2100);
}

int ReadMonth()
{
	return ReadIntInRange("Enter month as a number (1-12): ", 1, 12);
}

	// Menu 1: asks for a month and year, then reports the wind speed for it.
void DoWindReport(const WeatherDataStore& store)
{
	int month = ReadMonth();
	int year = ReadYear();

	cout << endl;

	ReportWindForMonth(store, year, month);
}

	// Menu 2: asks for a year, then reports each month's temperature.
void DoTemperatureReport(const WeatherDataStore& store)
{
	int year = ReadYear();

	cout << endl;

	ReportTemperatureForYear(store, year);
}

	// Menu 3: asks for a month only, because this option covers that month in
	// every year that has been loaded, so a year would have no meaning here.
void DoCorrelationReport(const WeatherDataStore& store)
{
	int month = ReadMonth();

	cout << endl;

	ReportCorrelationsForMonth(store, month);
}

	// Menu 4: asks for a year, then writes that year's figures to a file.
void DoFileOutput(const WeatherDataStore& store)
{
		// Keep asking until a year is given that actually has readings. Writing
		// a file for a year with no data would leave the user with a file that
		// says only "No Data", after being told the data had been written.
	int year = 0;
	bool haveYear = false;

	while(!haveYear)
	{
		year = ReadYear();

		if(HasDataForYear(store, year))
		{
			haveYear = true;
		}
		else
		{
			cout << "There are no readings for " << year
			     << ". Please enter a year that has data." << endl;
		}
	}

	if(WriteYearToFile(store, year, OUTPUT_FILE_NAME))
	{
		cout << "Data for " << year << " written to " << OUTPUT_FILE_NAME << endl;
	}
	else
	{
		cout << "Could not write to " << OUTPUT_FILE_NAME << endl;
	}
}
