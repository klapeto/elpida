//
// Created by klapeto on 13/8/2024.
//

#ifndef ELPIDA_MATRIX_HPP
#define ELPIDA_MATRIX_HPP

#include <valarray>

namespace Elpida
{

	class Matrix final
	{
	public:

		[[nodiscard]]
		Matrix Multiply(const Matrix& other) const;

		[[nodiscard]]
		double Get(std::size_t i, std::size_t j) const
		{
			return _values[i * _columns + j];
		}

		double& Get(std::size_t i, std::size_t j)
		{
			return _values[i * _columns + j];
		}

		Matrix()
				:Matrix(0, 0)
		{
		}

		Matrix(size_t rows, size_t columns);

		template<class ... T>
		explicit Matrix(size_t rows, size_t columns, const T ... values)
				:_rows(rows), _columns(columns), _values{ values... }
		{

		}

		Matrix(const Matrix&) = delete;
		Matrix(Matrix&&) noexcept = default;
		Matrix& operator=(const Matrix&) = delete;
		Matrix& operator=(Matrix&&) noexcept = default;
		~Matrix() = default;
	private:
		std::valarray<double> _values;
		std::size_t _rows;
		std::size_t _columns;
	};

} // Elpida

#endif //ELPIDA_MATRIX_HPP
