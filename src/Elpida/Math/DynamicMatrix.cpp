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
// Created by klapeto on 13/8/2024.
//

#include "Elpida/Math/DynamicMatrix.hpp"

namespace Elpida::Math
{
	DynamicMatrix::DynamicMatrix(size_t rows, size_t columns)
			:_rows(rows), _columns(columns)
	{
		_values.resize(rows * columns);
	}

	DynamicMatrix DynamicMatrix::Multiply(const DynamicMatrix& other) const
	{
		DynamicMatrix returnMatrix(_rows, other._columns);
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