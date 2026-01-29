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
// Created by klapeto on 7/3/2024.
//

#include "Elpida/Svg/SvgBlender.hpp"
#include "Elpida/Svg/SvgColor.hpp"

#include <cmath>

namespace Elpida
{
	static SvgFloat ClampColor(SvgFloat color)
	{
		if (color > 1.0) color = 1.0;
		if (color < 0.0) color = 0.0;
		return color;
	}

	static SvgFloat Normal(SvgFloat Cb, SvgFloat Cs)
	{
		return ClampColor(Cs);
	}

	static SvgFloat Multiply(SvgFloat Cb, SvgFloat Cs)
	{
		return ClampColor(Cb * Cs);
	}

	static SvgFloat Screen(SvgFloat Cb, SvgFloat Cs)
	{
		return ClampColor(Cb + Cs - (Cb * Cs));
	}

	static SvgFloat HardLight(SvgFloat Cb, SvgFloat Cs)
	{
		if (Cs <= 0.5)
		{
			return Multiply(Cb, 2 * Cs);
		}
		else
		{
			return Screen(Cb, (2 * Cs) - SvgFloat(1.0));
		}
	}

	static SvgFloat Overlay(SvgFloat Cb, SvgFloat Cs)
	{
		return HardLight(Cs, Cb);
	}

	static SvgFloat Darken(SvgFloat Cb, SvgFloat Cs)
	{
		return ClampColor(std::min(Cs, Cb));
	}

	static SvgFloat Lighten(SvgFloat Cb, SvgFloat Cs)
	{
		return ClampColor(std::max(Cs, Cb));
	}

	static SvgFloat ColorDodge(SvgFloat Cb, SvgFloat Cs)
	{
		if (Cb == 0.0)
		{
			return 0.0;
		}
		else if (Cs == 1.0)
		{
			return 1.0;
		}
		else
		{
			return ClampColor(std::min(SvgFloat(1.0), Cb / (SvgFloat(1.0) - Cs)));
		}
	}

	static SvgFloat ColorBurn(SvgFloat Cb, SvgFloat Cs)
	{
		if (Cb == 1.0)
		{
			return 1.0;
		}
		else if (Cs == 0.0)
		{
			return 0.0;
		}
		else
		{
			return ClampColor(SvgFloat(1.0) - std::min(SvgFloat(1.0), (SvgFloat(1.0) - Cb) / Cs));
		}
	}

	static SvgFloat SoftLight(SvgFloat Cb, SvgFloat Cs)
	{
		auto d = [](SvgFloat c)
		{
		  if (c <= 0.25)
		  {
			  return ((16 * c - 12) * c + 4) * c;
		  }
		  else
		  {
			  return std::sqrt(c);
		  }
		};

		if (Cs <= 0.5)
		{
			return ClampColor(Cb - (SvgFloat(1.0) - 2 * Cs) * Cb * (SvgFloat(1.0) - Cb));
		}
		else
		{
			return ClampColor(Cb + (2 * Cs - SvgFloat(1.0)) * (d(Cb) - Cb));
		}
	}

	static SvgFloat Difference(SvgFloat Cb, SvgFloat Cs)
	{
		return ClampColor(std::abs(Cb - Cs));
	}

	static SvgFloat Exclusion(SvgFloat Cb, SvgFloat Cs)
	{
		return ClampColor(Cb + Cs - 2 * Cb * Cs);
	}

	SvgBlender::SvgBlender(SvgBlendMode mode)
	{
		switch (mode)
		{
		case SvgBlendMode::Normal:
			_blender = Normal;
			break;
		case SvgBlendMode::Multiply:
			_blender = Multiply;
			break;
		case SvgBlendMode::Screen:
			_blender = Screen;
			break;
		case SvgBlendMode::Overlay:
			_blender = Overlay;
			break;
		case SvgBlendMode::Darken:
			_blender = Darken;
			break;
		case SvgBlendMode::Lighten:
			_blender = Lighten;
			break;
		case SvgBlendMode::ColorDodge:
			_blender = ColorDodge;
			break;
		case SvgBlendMode::ColorBurn:
			_blender = ColorBurn;
			break;
		case SvgBlendMode::HardLight:
			_blender = HardLight;
			break;
		case SvgBlendMode::SoftLight:
			_blender = SoftLight;
			break;
		case SvgBlendMode::Difference:
			_blender = Difference;
			break;
		case SvgBlendMode::Exclusion:
			_blender = Exclusion;
			break;
		default:
			_blender = Normal;
		}
	}
} // Elpida