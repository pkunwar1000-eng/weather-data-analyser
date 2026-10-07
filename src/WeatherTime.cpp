#include "WeatherTime.h"

Time::Time()
	: m_hour(0), m_minute(0)
{
}

Time::Time(int hour, int minute)
	: m_hour(hour), m_minute(minute)
{
}

void Time::SetHour(int hour)
{
	m_hour=hour;
}

void Time::SetMinute(int minute)
{
	m_minute=minute;
}

int Time::GetHour() const
{
	return m_hour;
}

int Time::GetMinute() const
{
	return m_minute;
}
