#include "Statistics.h"
#include <cmath>

float Sum(const Vector<float>& values)
{
	float total=0.0;

	for(int i=0;i<values.GetSize();i++)
	{
		total=total+values[i];
	}

	return total;
}

float Average(const Vector<float>& values)
{
	if(values.GetSize()==0)
	{
		return 0.0;
	}

	return Sum(values)/values.GetSize();
}

float SampleStandardDeviation(const Vector<float>& values)
{
	if(values.GetSize()<2)
	{
		return 0.0;
	}

	float average=Average(values);
	float total=0.0;

	for(int i=0;i<values.GetSize();i++)
	{
		float difference=values[i]-average;
		total=total+(difference*difference);
	}

	return sqrt(total/(values.GetSize()-1));
}

float mad(const Vector<float>& values)
{
	if(values.GetSize()==0)
	{
		return 0.0;
	}

	float average=Average(values);
	float total=0.0;

	for(int i=0;i<values.GetSize();i++)
	{
		float difference=values[i]-average;

		// Take the distance as a positive number, so that values above the
		// mean and values below it do not cancel each other out.
		if(difference<0.0)
		{
			difference=-difference;
		}

		total=total+difference;
	}

	return total/values.GetSize();
}

float sPCC(const Vector<float>& x, const Vector<float>& y)
{
	// The values are pairs, so x[i] and y[i] have to be readings from the same
	// observation. If the two lists are different lengths they cannot be pairs.
	if(x.GetSize()!=y.GetSize())
	{
		return 0.0;
	}

	// Two pairs at least are needed before there is any relationship to find.
	if(x.GetSize()<2)
	{
		return 0.0;
	}

	float averageX=Average(x);
	float averageY=Average(y);

	float top=0.0;
	float spreadX=0.0;
	float spreadY=0.0;

	for(int i=0;i<x.GetSize();i++)
	{
		// How far each of the pair is from its own average.
		float differenceX=x[i]-averageX;
		float differenceY=y[i]-averageY;

		// If both are above their average, or both below, this product is
		// positive and the total grows. If one is above while the other is
		// below, it is negative and the total shrinks. So the total ends up
		// large and positive when the two lists move together.
		top=top+(differenceX*differenceY);

		// The spread of each list on its own, used to scale the answer so it
		// always lands between -1 and 1.
		spreadX=spreadX+(differenceX*differenceX);
		spreadY=spreadY+(differenceY*differenceY);
	}

	float bottom=sqrt(spreadX)*sqrt(spreadY);

	// If every value in one of the lists is the same, that list has no spread,
	// so there is nothing for the other list to move with. Dividing here would
	// be dividing by zero.
	//
	// Floating point values are not exact, so a value that should be zero can
	// come out as a very small number instead. Testing against a tiny threshold
	// rather than against zero catches those as well.
	if(bottom<0.0000001)
	{
		return 0.0;
	}

	return top/bottom;
}
