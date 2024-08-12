//
// Created by klapeto on 13/8/2024.
//

#include "Matrix.hpp"
#include "Elpida/Core/ElpidaException.hpp"

namespace Elpida
{
	Matrix::Matrix(size_t rows, size_t columns)
			:_rows(rows), _columns(columns)
	{
		_values.resize(rows * columns);
	}

	Matrix Matrix::Multiply(const Matrix& other) const
	{
		if (_columns != other._rows) throw ElpidaException("Matrix multiplication requires A's columns to be equal to B's rows");
		Matrix returnMatrix(_rows, other._columns);
		for (std::size_t i = 0; i < _rows; ++i)
		{
			for (std::size_t j = 0; j < other._columns; ++j)
			{
				double product = 0.0;
				for (std::size_t k = 0; k < _columns; ++k)
				{
					product += Get(i, k) * other.Get(k, j);
				}
				returnMatrix.Get(i, j) = product;
			}
		}
		return returnMatrix;
	}
} // Elpida