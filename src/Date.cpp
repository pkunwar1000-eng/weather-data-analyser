#include "Date.h"

Date::Date()
	: m_day(0), m_month(0), m_year(0)
{
}

Date::Date(int day, int month, int year)
	: m_day(day), m_month(month), m_year(year)
{
}

void Date::SetDay(int day)
{
	m_day=day;
}

void Date::SetMonth(int month)
{
	m_month=month;
}

void Date::SetYear(int year)
{
	m_year=year;
}

int Date::GetDay() const
{
	return m_day;
}

int Date::GetMonth() const
{
	return m_month;
}

int Date::GetYear() const
{
	return m_year;
}

bool Date::operator<(const Date& other) const
{
	// Compare year first. If the years differ, that decides it.
	if(m_year != other.m_year)
	{
		return m_year < other.m_year;
	}

	// Same year, so compare month next.
	if(m_month != other.m_month)
	{
		return m_month < other.m_month;
	}

	// Same year and month, so the day decides it.
	return m_day < other.m_day;
}

bool Date::operator==(const Date& other) const
{
	return m_day == other.m_day &&
	       m_month == other.m_month &&
	       m_year == other.m_year;
}

std::ostream& operator<<(std::ostream& output, const Date& date)
{
	output << date.GetDay() << "/" << date.GetMonth() << "/" << date.GetYear();
	return output;
}
