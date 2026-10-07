#ifndef ICT283_STATISTICS_H
#define ICT283_STATISTICS_H

#include "Vector.h"

	/**
	 * @file Statistics.h
	 * @brief Generic statistical helper routines that operate on a Vector<float>.
	 *
	 * These routines are non-member functions, not methods of any data class.
	 * They depend only on a Vector<float>, so they know nothing about weather,
	 * dates, or any application type. This keeps the statistics reusable: the
	 * same functions can compute the average of any list of float values, in
	 * this assignment or anywhere else.
	 *
	 * The caller is responsible for collecting the values of interest into a
	 * Vector<float> before calling these routines.
	 *
	 * @author Prasanna Kunwar
	 * @version 02
	 * @date 25/06/2026 Prasanna Kunwar, Created for Assignment 1
	 * @date 22/07/2026 Prasanna Kunwar, Added mad and sPCC for Lab 10 (e) / Assignment 2
	 */

	/**
	 * @brief Returns the sum of all values in the Vector.
	 *
	 * @param values - the list of values to add up
	 * @return float - the total of all values, or 0 if the Vector is empty
	 */
float Sum(const Vector<float>& values);

	/**
	 * @brief Returns the arithmetic mean (average) of the values.
	 *
	 * @param values - the list of values
	 * @return float - the average, or 0 if the Vector is empty
	 */
float Average(const Vector<float>& values);

	/**
	 * @brief Returns the sample standard deviation of the values.
	 *
	 * Uses the sample formula (division by n - 1). At least two values are
	 * required for a meaningful result.
	 *
	 * @param values - the list of values
	 * @return float - the sample standard deviation, or 0 if fewer than two values
	 */
float SampleStandardDeviation(const Vector<float>& values);


	/**
	 * @brief Returns the mean absolute deviation (MAD) of the values.
	 *
	 * This is the average distance of the values from their own mean. Every
	 * distance is taken as a positive number, so values above and below the
	 * mean do not cancel each other out.
	 *
	 * It measures spread, like the standard deviation does, but it does not
	 * square the distances. Squaring makes a few very unusual readings count
	 * for much more than the rest, so the standard deviation reacts strongly
	 * to them while the mean absolute deviation does not. Reporting both
	 * shows whether the spread comes from steady variation or from a small
	 * number of extreme readings.
	 *
	 * @param values - the list of values
	 * @return float - the mean absolute deviation, or 0 if the Vector is empty
	 */
float mad(const Vector<float>& values);

	/**
	 * @brief Returns the sample Pearson Correlation Coefficient of two lists.
	 *
	 * This measures how strongly two lists of readings move together. The
	 * result is between -1 and 1:
	 *   1 means that when one value goes up the other goes up as well, in a
	 *     perfectly straight line;
	 *   0 means there is no straight-line relationship between them;
	 *  -1 means that when one value goes up the other goes down, in a
	 *     perfectly straight line.
	 *
	 * The two lists are pairs of readings, so x[i] and y[i] must come from the
	 * same observation. The caller is responsible for building the two Vectors
	 * so that this is true. If the two lists are different lengths they cannot
	 * be pairs, so 0 is returned.
	 *
	 * If either list has no spread at all (every value in it is the same) then
	 * there is nothing for the other list to move with, and the formula would
	 * divide by zero. In that case 0 is returned.
	 *
	 * @param x - the first list of readings
	 * @param y - the second list of readings, paired with x
	 * @return float - the correlation between -1 and 1, or 0 if it cannot be found
	 */
float sPCC(const Vector<float>& x, const Vector<float>& y);

#endif
