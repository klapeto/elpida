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

#ifndef ELPIDA_MATH_DYNAMICMATRIX_HPP
#define ELPIDA_MATH_DYNAMICMATRIX_HPP

#include <valarray>

namespace Elpida::Math
{

	class DynamicMatrix final
	{
	public:

		[[nodiscard]]
		DynamicMatrix Multiply(const DynamicMatrix& other) const;

		[[nodiscard]]
		double Get(std::size_t i, std::size_t j) const
		{
			return _values[i * _columns + j];
		}

		double& Get(std::size_t i, std::size_t j)
		{
			return _values[i * _columns + j];
		}

		DynamicMatrix()
				:DynamicMatrix(0, 0)
		{
		}

		DynamicMatrix(size_t rows, size_t columns);

		template<class ... T>
		explicit DynamicMatrix(size_t rows, size_t columns, const T ... values)
				:_rows(rows), _columns(columns), _values{ values... }
		{

		}

		DynamicMatrix(const DynamicMatrix&) = delete;
		DynamicMatrix(DynamicMatrix&&) noexcept = default;
		DynamicMatrix& operator=(const DynamicMatrix&) = delete;
		DynamicMatrix& operator=(DynamicMatrix&&) noexcept = default;
		~DynamicMatrix() = default;
	private:
		std::valarray<double> _values;
		std::size_t _rows;
		std::size_t _columns;
	};

} // Elpida::Math

#endif //ELPIDA_MATH_DYNAMICMATRIX_HPP
