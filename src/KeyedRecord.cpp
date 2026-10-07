#include "KeyedRecord.h"

KeyedRecord::KeyedRecord()
	: key(0), date(), time(), windSpeed(0.0), temperature(0.0), solarRadiation(0.0),
	  hasWindSpeed(true), hasTemperature(true), hasSolarRadiation(true)
{
}

KeyedRecord::KeyedRecord(const WeatherRecord& record)
	: key(0), date(record.date), time(record.time),
	  windSpeed(record.windSpeed),
	  temperature(record.temperature),
	  solarRadiation(record.solarRadiation),
	  hasWindSpeed(record.hasWindSpeed),
	  hasTemperature(record.hasTemperature),
	  hasSolarRadiation(record.hasSolarRadiation)
{
	// Work out the key from the date and time this reading was taken.
	key = WeatherKey::ReadingKey(record.date, record.time);
}

bool KeyedRecord::operator<(const KeyedRecord& other) const
{
	return key < other.key;
}
