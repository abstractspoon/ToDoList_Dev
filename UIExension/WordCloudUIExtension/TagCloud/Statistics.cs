using System;
using System.Collections.Generic;

namespace TagCloud
{
	/// <summary>
	/// Statistical functions: mean value and standard deviation
	/// </summary>
	public static class Statistics
	{

    /// <summary>
    /// Calculates the mean value of a number of values
    /// and returns it.
    /// </summary>
    /// <param name="values"></param>
    /// <returns></returns>
		public static double Mean(IEnumerable<double> values)
		{
			double sum = 0;
			int count = 0;

			foreach (double d in values)
			{
				sum += d;
				count++;
			}
			return (count == 0)? 0 : sum / count;
		}

		/// <summary>
    /// Calculates the mean value and the standard deviation 
    /// of a number of values and returns the standard deviation.
    /// The mean value is returned by reference.
    /// </summary>
		/// <param name="values"></param>
		/// <param name="mean"></param>
		/// <returns></returns>
		public static double StandardDeviation(IEnumerable<double> values, out double mean)
		{
			mean = Statistics.Mean(values);
			double sum = 0;
			int count = 0;

			foreach (double d in values)
			{
				double diff = d - mean;
				sum += (diff * diff);
				count++;
			}
      return (count == 0)? 0 : Math.Sqrt(sum / count);
		}

	}
}