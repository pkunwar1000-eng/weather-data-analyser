#include "WeatherDataReader.h"
#include <fstream>
#include <sstream>

// Only the specific names actually used are brought in, instead of exposing
// the whole std namespace with "using namespace std;".
using std::string;
using std::ifstream;
using std::stringstream;
using std::getline;

// Anonymous-namespace helpers: private to this file, not part of the interface.

namespace
{
		// Removes a trailing carriage return left over from Windows line endings.
	void StripCarriageReturn(string& text)
	{
		if(text.length()>0 && text[text.length()-1]=='\r')
		{
			text.erase(text.length()-1);
		}
	}

		// Returns true if text holds a value that reads as a number.
	bool IsNumber(const string& text)
	{
		if(text=="")
		{
			return false;
		}

		stringstream input(text);
		double value;
		input>>value;

		return !input.fail();
	}

		// Splits one CSV line into fields. Returns the number of fields found.
		// A trailing separator is treated as one more empty field, so a line
		// that ends with a blank value (for example a missing last column) is
		// not counted as one field short and wrongly dropped.
	int SplitLine(const string& line, string fields[], int maxFields)
	{
		stringstream lineStream(line);
		string field;
		int count=0;

		while(count<maxFields && getline(lineStream,field,','))
		{
			StripCarriageReturn(field);
			fields[count]=field;
			count++;
		}

			// getline does not produce a final empty field when the line ends
			// with a separator, so add it back here when there is room.
		if(count<maxFields && line.length()>0)
		{
			string trimmed=line;
			StripCarriageReturn(trimmed);
			if(trimmed.length()>0 && trimmed[trimmed.length()-1]==',')
			{
				fields[count]="";
				count++;
			}
		}

		return count;
	}

		// Finds the index of a named column in the header. Returns -1 if absent.
	int FindColumn(const string headings[], int count, const string& name)
	{
		for(int i=0;i<count;i++)
		{
			if(headings[i]==name)
			{
				return i;
			}
		}

		return -1;
	}

		// Parses a WAST value such as "1/03/2014 9:00" into a Date and a Time.
	bool ParseDateTime(const string& text, Date& date, Time& time)
	{
		string datePart;
		string timePart;

		stringstream fullStream(text);
		fullStream>>datePart>>timePart;

		if(datePart=="" || timePart=="")
		{
			return false;
		}

		int day,month,year,hour,minute;
		char slash1,slash2,colon;

		stringstream dateStream(datePart);
		dateStream>>day>>slash1>>month>>slash2>>year;

		stringstream timeStream(timePart);
		timeStream>>hour>>colon>>minute;

		if(!dateStream || !timeStream)
		{
			return false;
		}

		date.SetDay(day);
		date.SetMonth(month);
		date.SetYear(year);

		time.SetHour(hour);
		time.SetMinute(minute);

		return true;
	}

		// Reads a numeric field into a float.
	float ToFloat(const string& text)
	{
		stringstream stream(text);
		float value=0.0;
		stream>>value;
		return value;
	}
}

bool ReadDataSourceNames(const string& sourceFileName, Vector<string>& dataFileNames)
{
	ifstream infile(sourceFileName.c_str());

	if(!infile)
	{
		return false;
	}

	string name;

		// Read every line; each non-empty line is one data file name.
	while(getline(infile,name))
	{
		StripCarriageReturn(name);

		if(name!="")
		{
			dataFileNames.Add(name);
		}
	}

	return dataFileNames.GetSize()>0;
}

bool LoadWeatherData(const string& dataFileName, Vector<WeatherRecord>& weatherData)
{
	const int MAX_COLUMNS=50;

	ifstream infile(dataFileName.c_str());

	if(!infile)
	{
		return false;
	}

	string line;

	if(!getline(infile,line))
	{
		return false;
	}

	string headings[MAX_COLUMNS];
	int columnCount=SplitLine(line,headings,MAX_COLUMNS);

	int wastColumn=FindColumn(headings,columnCount,"WAST");
	int speedColumn=FindColumn(headings,columnCount,"S");
	int solarColumn=FindColumn(headings,columnCount,"SR");
	int tempColumn=FindColumn(headings,columnCount,"T");

	if(wastColumn==-1 || speedColumn==-1 || solarColumn==-1 || tempColumn==-1)
	{
		return false;
	}

	int highestColumn=wastColumn;
	if(speedColumn>highestColumn) highestColumn=speedColumn;
	if(solarColumn>highestColumn) highestColumn=solarColumn;
	if(tempColumn>highestColumn) highestColumn=tempColumn;

		// Append directly into the caller's Vector so that records from files
		// loaded earlier are preserved. This is what allows several files to be
		// combined into one Vector by calling this routine once per file.
	while(getline(infile,line))
	{
		string fields[MAX_COLUMNS];
		int fieldCount=SplitLine(line,fields,MAX_COLUMNS);

			// Skip any short row that does not reach all needed columns.
		if(fieldCount<=highestColumn)
		{
			continue;
		}

		// A row is kept when its date and time parse and at least one of the
		// three measured fields holds a number. A field that is blank in the
		// file is recorded as not present, so the reports can leave it out
		// rather than treat a missing value as a real reading of zero. A row
		// with no measured values at all is of no use and is skipped.
		bool windPresent  = IsNumber(fields[speedColumn]);
		bool tempPresent  = IsNumber(fields[tempColumn]);
		bool solarPresent = IsNumber(fields[solarColumn]);

		if(windPresent || tempPresent || solarPresent)
		{
			WeatherRecord record;

			if(ParseDateTime(fields[wastColumn],record.date,record.time))
			{
				record.hasWindSpeed      = windPresent;
				record.hasTemperature    = tempPresent;
				record.hasSolarRadiation = solarPresent;

					// A missing field is left at zero and flagged as absent
					// above, so nothing reads it as a genuine measurement.
				record.windSpeed      = windPresent  ? ToFloat(fields[speedColumn]) : 0.0f;
				record.temperature    = tempPresent  ? ToFloat(fields[tempColumn])  : 0.0f;
				record.solarRadiation = solarPresent ? ToFloat(fields[solarColumn]) : 0.0f;

				weatherData.Add(record);
			}
		}
	}

	return true;
}
