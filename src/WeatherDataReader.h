#ifndef ICT283_WEATHER_DATA_READER_H
#define ICT283_WEATHER_DATA_READER_H

#include <string>
#include "Vector.h"
#include "WeatherRecord.h"

	/**
	 * @file WeatherDataReader.h
	 * @brief Routines for loading weather records from the input data file.
	 *
	 * These are non-member functions that handle all file input for the program.
	 * Input is deliberately kept out of the data classes (Date, Time,
	 * WeatherRecord, Vector) so that those classes stay free of I/O. If the input
	 * format changes later, only this module needs to change.
	 *
	 * The program never asks the user for a file name. The names of the actual
	 * data files are read from data_source.txt (one file name per line), and each
	 * data file is read from the data folder. Columns are located by their header
	 * label, so the routines work regardless of the order of the columns in the
	 * file. The list in data_source.txt is not assumed to be in any sort order.
	 *
	 * @author Prasanna Kunwar
	 * @version 04
	 * @date 22/06/2026 Created for Assignment 1
	 * @date 07/07/2026 Lab 8 Q4: multiple file names read; loading now appends
	 */

	/**
	 * @brief Reads every data file name listed in data_source.txt.
	 *
	 * Each non-empty line of the source file is treated as one data file name and
	 * added to dataFileNames in the order encountered. Blank lines are ignored.
	 * The order of the names is not significant to the caller.
	 *
	 * @param sourceFileName - path to the data_source.txt file
	 * @param dataFileNames - Vector that receives the data file names read
	 * @return bool - true if at least one name was read, false if the file could
	 *                not be opened or contained no names
	 */
bool ReadDataSourceNames(const std::string& sourceFileName, Vector<std::string>& dataFileNames);

	/**
	 * @brief Loads all valid weather records from a CSV data file, appending them.
	 *
	 * Reads the header row to locate the WAST, S, SR and T columns by name, then
	 * loads each following row. A row is only stored if its date/time parses and
	 * its three values are numeric. Values are stored in the file's own units.
	 *
	 * Records are ADDED to weatherData; any records already present are kept. This
	 * lets the caller load several files into one Vector by calling this routine
	 * once per file.
	 *
	 * @param dataFileName - path to the CSV data file
	 * @param weatherData - Vector that receives the loaded records (appended)
	 * @return bool - true if the file was read and the required columns were found
	 */
bool LoadWeatherData(const std::string& dataFileName, Vector<WeatherRecord>& weatherData);

#endif
