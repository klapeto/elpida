//
// Created by klapeto on 12/8/2024.
//

#ifndef ELPIDA_STATICMATRIX_HPP
#define ELPIDA_STATICMATRIX_HPP

#include <cstddef>

namespace Elpida
{

	template<std::size_t R, std::size_t C>
	class StaticMatrix
	{
	public:

		template<std::size_t Oc>
		StaticMatrix<R, Oc> Multiply(const StaticMatrix<C, Oc>& other) const
		{
			StaticMatrix<R, Oc> returnMatrix;
			for (std::size_t i = 0; i < R; ++i)
			{
				for (std::size_t j = 0; j < Oc; ++j)
				{
					double product = 0.0;
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

		StaticMatrix()
				:_values{ 0.0 }
		{

		}

		template<class ... T>
		explicit StaticMatrix(const T ... values)
				:_values{ values... }
		{

		}

	private:
		double _values[R * C];
	};

} // Elpida

#endif //ELPIDA_STATICMATRIX_HPP
