/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGGRADIENT_HPP
#define ELPIDA_SVG_SVGGRADIENT_HPP

#include "Elpida/Svg/SvgGradientStop.hpp"
#include "Elpida/Svg/SvgTransform.hpp"
#include "Elpida/Svg/SvgSpreadType.hpp"
#include "Elpida/Svg/SvgGradientUnits.hpp"
#include "Elpida/Svg/SvgGradientType.hpp"

#include <string>
#include <vector>

namespace Elpida
{

	class SvgGradient : public SvgElement
	{
	public:
		union GradientData
		{
			struct
			{
				SvgLength x1;
				SvgLength y1;
				SvgLength x2;
				SvgLength y2;
			} linear;
			struct
			{
				SvgLength cx;
				SvgLength cy;
				SvgLength r;
				SvgLength fx;
				SvgLength fy;
			} radial;

			GradientData()
			{
			}
		};

		[[nodiscard]]
		const std::string& GetHref() const
		{
			return _href;
		}

		const std::vector<SvgGradientStop>& GetStops() const
		{
			return _stops;
		}

		const SvgTransform& GetGradientTransform() const
		{
			return _gradientTransform;
		}

		SvgSpreadType GetSpreadType() const
		{
			return _spreadType;
		}

		SvgGradientUnits GetUnits() const
		{
			return _units;
		}

		[[nodiscard]]
		SvgGradientType GetType() const
		{
			return _type;
		}

		[[nodiscard]]
		const GradientData& GetData() const
		{
			return _data;
		}

		explicit SvgGradient(const XmlElement& element, SvgDocument& document);

	protected:
		std::string _href;
		std::vector<SvgGradientStop> _stops;
		SvgTransform _gradientTransform;
		SvgSpreadType _spreadType;
		SvgGradientUnits _units;
		SvgGradientType _type;
		GradientData _data;

		void ParseAsRadial();
		void ParseAsLinear();
	};
} // Elpida

#endif //ELPIDA_SVG_SVGGRADIENT_HPP
