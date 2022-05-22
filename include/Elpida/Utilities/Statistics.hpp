/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 7/2/21.
//

#ifndef SRC_ELPIDA_UTILITIES_STATISTICS_HPP
#define SRC_ELPIDA_UTILITIES_STATISTICS_HPP

#include <cmath>

namespace Elpida
{
	struct BasicStatistics
	{
		size_t sampleSize;
		double tau;
		double mean;
		double min;
		double max;
		double marginOfError;
		double standardDeviation;
	};


	class Statistics
	{
	public:

		template<typename T, typename TCallable>
		static BasicStatistics calculateBasicStatistics(const T& population, TCallable xGetter = [](auto x){ return x; })
		{
			double mean = 0.0;
			double max = std::numeric_limits<double>::min();
			double min = std::numeric_limits<double>::max();
			size_t n = population.size();

			for (auto& x: population)
			{
				auto value = xGetter(x);
				mean += value;
				max = std::max(max, value);
				min = std::min(min, value);
			}
			mean /= n;

			double deviation = 0.0;
			for (auto& x : population)
			{
				deviation += std::pow(xGetter(x) - mean, 2.0);
			}

			deviation = std::sqrt(deviation / n);

			return
				{
					n,
					tau(n),
					mean,
					min,
					max,
					deviation / std::sqrt(n),
					deviation
				};
		}

		template<typename T>
		static constexpr double tau(T size)
		{
			constexpr const std::pair<double, double> table[] = {
				{ 3, 1.1511 }, { 4, 1.4250 }, { 5, 1.5712 },
				{ 6, 1.6563 }, { 7, 1.7110 }, { 8, 1.7491 },
				{ 9, 1.7770 }, { 10, 1.7984 }, { 11, 1.8153 },
				{ 12, 1.8290 }, { 13, 1.8403 }, { 14, 1.8498 },
				{ 15, 1.8579 }, { 16, 1.8649 }, { 17, 1.8710 },
				{ 18, 1.8764 }, { 19, 1.8811 }, { 20, 1.8853 },
				{ 21, 1.8891 }, { 22, 1.8926 }, { 23, 1.8957 },
				{ 24, 1.8985 }, { 25, 1.9011 }, { 26, 1.9035 },
				{ 27, 1.9057 }, { 28, 1.9078 }, { 29, 1.9096 },
				{ 30, 1.9114 }, { 31, 1.9130 }, { 32, 1.9146 },
				{ 33, 1.9160 }, { 34, 1.9174 }, { 35, 1.9186 },
				{ 36, 1.9198 }, { 37, 1.9209 }, { 38, 1.9220 },
				{ 39, 1.9230 }, { 40, 1.9240 }, { 42, 1.9257 },
				{ 44, 1.9273 }, { 46, 1.9288 }, { 48, 1.9301 },
				{ 50, 1.9314 }, { 55, 1.9340 }, { 60, 1.9362 },
				{ 65, 1.9381 }, { 70, 1.9397 }, { 80, 1.9423 },
				{ 90, 1.9443 }, { 100, 1.9459 }, { 200, 1.9530 },
				{ 500, 1.9572 }, { 1000, 1.9586 }, { 5000, 1.9597 }
			};

			constexpr size_t tableSize = sizeof(table) / sizeof(*table);

			size_t i = tableSize / 2;

			size_t j = 4;

			// binary search the table
			while (true)
			{
				if (i == 0) return table[0].second;
				if (i >= tableSize) return table[tableSize - 1].second;
				if (i < tableSize - 1
					&& table[i].first <= size
					&& table[i + 1].first > size)
					return table[i].second;

				if (table[i].first < size)
				{
					i += std::max(static_cast<size_t>(1), tableSize / j);;
				}
				else
				{
					i -= std::max(static_cast<size_t>(1), tableSize / j);
				}
				j <<= 1;
			}
		}

		template<typename T, typename TCallable>
		static double arithmeticMean(T population, TCallable xGetter = [](auto x){ return x; })
		{
			double mean = 0.0;
			for (auto& x : population)
			{
				mean += xGetter(x);
			}

			mean /= population.size();

			return mean;
		}

		template<typename T, typename TCallable>
		static double harmonicMean(T population, TCallable xGetter = [](auto x){ return x; })
		{
			double mean = 0.0;
			for (auto& x : population)
			{
				mean += 1.0 / xGetter(x);
			}

			return population.size() / mean;
		}

		template<typename T>
		static double marginOfError(double standardDeviation, T size)
		{
			return standardDeviation / std::sqrt((double)size);
		}

		template<typename T, typename TCallable>
		static double standardDeviation(T population, TCallable xGetter = [](auto x){ return x; })
		{
			double mean = arithmeticMean(population, xGetter);

			double deviation = 0.0;
			for (auto& x : population)
			{
				deviation += std::pow(xGetter(x) - mean, 2.0);
			}

			deviation /= population.size();

			return std::sqrt(deviation);
		}
	};
}

#endif //SRC_ELPIDA_UTILITIES_STATISTICS_HPP
