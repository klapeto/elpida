//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 12/9/2024.
//

#ifndef ELPIDA_MATH_MATRIX_HPP
#define ELPIDA_MATH_MATRIX_HPP

#include <cstddef>

namespace Elpida::Math
{

	template<typename T, std::size_t R, std::size_t C>
	class Matrix
	{
		static_assert(R > 0);
		static_assert(C > 0);
	public:

		T operator[](std::size_t i) const
		{
			return _values[i];
		}

		Matrix<T, R, C> CalculateInverse() const
		{
			auto det = CalculateDeterminant();
			if (det < 1e-8) return Identity();

			auto adJoint = CalculateCofactors().CalculateTranspose();
			return adJoint * (1.0 / det);
		}

		Matrix<T, R, C> CalculateMinor() const
		{
			Matrix<T, R, C> out;
			for (std::size_t i = 0; i < R; ++i)
			{
				for (std::size_t j = 0; j < C; ++j)
				{
					out.Get(i, j) = CalculateRemovedRowColumn(i, j).CalculateDeterminant();
				}
			}

			return out;
		}

		Matrix<T, R, C> CalculateCofactors() const
		{
			auto minor = CalculateMinor();

			for (std::size_t i = 0, s = 1; i < R; ++i, ++s)
			{
				for (std::size_t j = s % 2; j < C; j += 2)
				{
					minor.Get(i, j) = -minor.Get(i, j);
				}
			}
			return minor;
		}

		Matrix<T, R, C> CalculateTranspose() const
		{
			static_assert(R == C);
			Matrix<T, C, R> out;

			for (std::size_t i = 0; i < R; ++i)
			{
				for (std::size_t j = i; j < C; ++j)
				{
					out.Get(i, j) = Get(j, i);
					out.Get(j, i) = Get(i, j);
				}
			}

			return out;
		}

		Matrix<T, R - 1, C - 1> CalculateRemovedRowColumn(std::size_t row, std::size_t column) const
		{
			Matrix<T, R - 1, C - 1> out;
			for (std::size_t i = 0, io = 0; i < R; ++i)
			{
				if (i == row) continue;
				for (std::size_t j = 0, jo = 0; j < C; ++j)
				{
					if (j == column) continue;

					out.Get(io, jo) = Get(i, j);

					jo++;
				}
				io++;
			}

			return out;
		}

		T CalculateDeterminant() const
		{
			static_assert(R == C);

			if constexpr (R > 2)
			{
				T det = 0.0;
				T sign = 1.0;
				for (std::size_t i = 0; i < C; ++i)
				{
					auto sub = CalculateRemovedRowColumn(0, i);

					det += sign * Get(0, i) * sub.CalculateDeterminant();

					sign = -sign;
				}

				return det;
			}
			else
			{
				return (_values[0] * _values[3]) - (_values[1] * _values[2]);
			}
		}

		Matrix<T, R, C> operator-(T value) const
		{
			auto copy = *this;
			for (std::size_t i = 0; i < R * C; ++i)
			{
				copy._values[i] -= value;
			}
			return copy;
		}

		Matrix<T, R, C>& operator-=(T value) const
		{
			for (std::size_t i = 0; i < R * C; ++i)
			{
				_values[i] -= value;
			}
			return *this;
		}

		Matrix<T, R, C> operator+(T value) const
		{
			auto copy = *this;
			for (std::size_t i = 0; i < R * C; ++i)
			{
				copy._values[i] += value;
			}
			return copy;
		}

		Matrix<T, R, C>& operator+=(T value) const
		{
			for (std::size_t i = 0; i < R * C; ++i)
			{
				_values[i] += value;
			}
			return *this;
		}

		Matrix<T, R, C>& operator*=(T value) const
		{
			for (std::size_t i = 0; i < R * C; ++i)
			{
				_values[i] *= value;
			}
			return *this;
		}

		Matrix<T, R, C> operator*(T value) const
		{
			auto copy = *this;
			for (std::size_t i = 0; i < R * C; ++i)
			{
				copy._values[i] *= value;
			}
			return copy;
		}

		template<std::size_t Oc>
		Matrix<T, R, Oc> operator*(const Matrix<T, C, Oc>& other) const
		{
			Matrix<T, R, Oc> returnMatrix;
			for (std::size_t i = 0; i < R; ++i)
			{
				for (std::size_t j = 0; j < Oc; ++j)
				{
					T product = 0.0;
					for (std::size_t k = 0; k < C; ++k)
					{
						product += Get(i, k) * other.Get(k, j);
					}
					returnMatrix.Get(i, j) = product;
				}
			}

			return returnMatrix;
		}

		[[nodiscard]]
		double Get(std::size_t r, std::size_t c) const
		{
			return _values[r * C + c];
		}

		double& Get(std::size_t r, std::size_t c)
		{
			return _values[r * C + c];
		}

		constexpr Matrix()
				:_values{ 0.0 }
		{

		}

		static constexpr Matrix<T, R, C> Identity()
		{
			Matrix<T, R, C> ret;

			for (std::size_t i = 0; i < R * C; i += C + 1)
			{
				ret._values[i] = 1.0;
			}

			return ret;
		}

		template<class ... TValue>
		static constexpr Matrix<T, R, C> WithOnes(const TValue... values)
		{
			Matrix<T, R, C> ret{ values... };

			auto argumentCount = sizeof...(values);
			if (argumentCount >= R * C)
			{
				return ret;
			}
			for (std::size_t i = argumentCount; i < (R * C) - argumentCount; ++i)
			{
				ret._values[i] = 1.0;
			}

			return ret;
		}

		template<class ... TValue>
		constexpr explicit Matrix(const TValue... values)
				:_values{ values... }
		{
		}

		Matrix(const Matrix<T, R, C>&) = default;
		Matrix& operator=(const Matrix<T, R, C>&) = default;
		Matrix(Matrix<T, R, C>&&) noexcept = default;
		Matrix& operator=(Matrix<T, R, C>&&) noexcept = default;
		~Matrix() = default;
	private:
		T _values[R * C];

		friend class Matrix<T, C, R>;
	};

} // Elpida

#endif //ELPIDA_MATH_MATRIX_HPP
