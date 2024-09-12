//
// Created by klapeto on 12/9/2024.
//

#ifndef ELPIDA_MATRIX_HPP
#define ELPIDA_MATRIX_HPP

#include <cstddef>

namespace Elpida
{

	template<typename T, std::size_t R, std::size_t C>
	class Matrix
	{
		static_assert(R > 0);
		static_assert(C > 0);
	public:

		Matrix<T, R - 1, C - 1> RemoveRowColumn(std::size_t row, std::size_t column) const
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

		T Determinant() const
		{
			static_assert(R == C);

			if constexpr (R > 2)
			{
				T det = 0.0;
				double sign = 1.0;
				for (std::size_t i = 0; i < C; ++i)
				{
					auto sub = RemoveRowColumn(0, i);

					det += sign * Get(0, i) * sub.Determinant();

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

		Matrix()
				:_values{ 0.0 }
		{

		}

		static constexpr Matrix<T, R, C> Identity()
		{
			Matrix<T, R, C> ret;

			for (std::size_t i = 0; i < R * C; i += C + 1)
			{
				ret._values[i] = 1;
			}

			return ret;
		}

		template<class ... TValue>
		explicit Matrix(const TValue ... values)
				:_values{ values... }
		{

		}

	private:
		T _values[R * C];
	};

} // Elpida

#endif //ELPIDA_MATRIX_HPP
