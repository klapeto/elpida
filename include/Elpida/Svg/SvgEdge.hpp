//
// Created by klapeto on 22/1/2024.
//

#ifndef ELPIDA_SVG_SVGRASTERIZEREDGE_HPP
#define ELPIDA_SVG_SVGRASTERIZEREDGE_HPP

#include "SvgPoint.hpp"

namespace Elpida
{
	class SvgEdge
	{
	public:
		[[nodiscard]]
		SvgPoint& GetA()
		{
			return _a;
		}

		[[nodiscard]]
		SvgPoint& GetB()
		{
			return _b;
		}

		[[nodiscard]]
		const SvgPoint& GetA() const
		{
			return _a;
		}

		[[nodiscard]]
		const SvgPoint& GetB() const
		{
			return _b;
		}

		[[nodiscard]]
		int GetDirection() const
		{
			return _direction;
		}

		SvgEdge()
			: _direction(0)
		{
		}

		SvgEdge(const SvgPoint& a, const SvgPoint& b, const int direction)
			: _a(a), _b(b), _direction(direction)
		{
		}

		SvgEdge(const SvgEdge&) = default;
		SvgEdge(SvgEdge&&) noexcept = default;
		SvgEdge& operator=(const SvgEdge&) noexcept = default;
		SvgEdge& operator=(SvgEdge&&) noexcept = default;
		~SvgEdge() = default;
	private:
		SvgPoint _a;
		SvgPoint _b;
		int _direction;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRASTERIZEREDGE_HPP
