#include "WeatherKey.h"

int WeatherKey::MonthKey(int year, int month)
{
	// Shift the year up by two decimal places and drop the month into the gap,
	// so 2014 and 6 become 201406. The month never reaches 100, so it always
	// fits in those two places and never spills over into the year.
	return (year * 100) + month;
}

int WeatherKey::YearOf(int monthKey)
{
	// The last two digits are the month, so dividing them away leaves the year.
	return monthKey / 100;
}

int WeatherKey::MonthOf(int monthKey)
{
	// The remainder after taking the year away is the month.
	return monthKey % 100;
}

long long WeatherKey::PlainReadingKey(const Date& date, const Time& time)
{
	// Join the date and the time into one number, biggest part first, so that
	// the number reads the same way a date does: year, month, day, hour,
	// minute. Each part is shifted up by enough decimal places to leave room
	// for the parts that follow it.
	long long key = 0;

	key = key + (long long)date.GetYear()  * 100000000LL;
	key = key + (long long)date.GetMonth() * 1000000LL;
	key = key + (long long)date.GetDay()   * 10000LL;
	key = key + (long long)time.GetHour()  * 100LL;
	key = key + (long long)time.GetMinute();

	return key;
}

long long WeatherKey::ReadingKey(const Date& date, const Time& time)
{
	return Hash(PlainReadingKey(date, time));
}

long long WeatherKey::Hash(long long value)
{
	// Radix conversion. Take the digits of the number one at a time and use
	// them as the factors of a polynomial whose base is HASH_BASE.
	//
	// Each time round, whatever has been built so far is multiplied by the
	// base and the next digit is added on. So the digits nearer the front of
	// the number end up multiplied by a bigger power of the base, which is
	// what makes two numbers with the same digits in a different order come
	// out differently.
	//
	// The remainder is taken every time round rather than only at the end.
	// Without that the running total would grow past what a long long can hold and
	// wrap around to a meaningless value.
	long long result = 0;
	long long remaining = value;

	while(remaining > 0)
	{
		long long digit = remaining % 10;

		result = ((result * HASH_BASE) + digit) % HASH_PRIME;

		remaining = remaining / 10;
	}

	return result;
}
