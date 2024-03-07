//
// Created by klapeto on 7/3/2024.
//

#include "Elpida/Svg/SvgBlender.hpp"
#include "Elpida/Svg/SvgColor.hpp"

#include <cmath>

namespace Elpida
{
	static double ClampColor(double color)
	{
		if (color > SvgColor::MaxValue) color = SvgColor::MaxValue;
		if (color < SvgColor::MinValue) color = SvgColor::MinValue;
		return color;
	}

	static double Normal(double Cb, double Cs)
	{
		return ClampColor(Cs);
	}

	static double Multiply(double Cb, double Cs)
	{
		return ClampColor(Cb * Cs);
	}


	static double Screen(double Cb, double Cs)
	{
		return ClampColor(Cb + Cs - (Cb * Cs));
	}

	static double HardLight(double Cb, double Cs)
	{
		if (Cs <= SvgColor::HalfValue)
		{
			return Multiply(Cb, 2 * Cs);
		}
		else
		{
			return Screen(Cb, (2 * Cs) - SvgColor::MaxValue);
		}
	}

	static double Overlay(double Cb, double Cs)
	{
		return HardLight(Cs, Cb);
	}

	static double Darken(double Cb, double Cs)
	{
		return ClampColor(std::min(Cs, Cb));
	}

	static double Lighten(double Cb, double Cs)
	{
		return ClampColor(std::max(Cs, Cb));
	}

	static double ColorDodge(double Cb, double Cs)
	{
		if (Cb == SvgColor::MinValue)
		{
			return SvgColor::MinValue;
		}
		else if (Cs == SvgColor::MaxValue)
		{
			return SvgColor::MaxValue;
		}
		else
		{
			return ClampColor(std::min(SvgColor::MaxValue, Cb / (SvgColor::MaxValue - Cs)));
		}
	}

	static double ColorBurn(double Cb, double Cs)
	{
		if (Cb == SvgColor::MaxValue)
		{
			return SvgColor::MaxValue;
		}
		else if (Cs == SvgColor::MinValue)
		{
			return SvgColor::MinValue;
		}
		else
		{
			return ClampColor(SvgColor::MaxValue - std::min(SvgColor::MaxValue, (SvgColor::MaxValue - Cb) / Cs));
		}
	}

	static double SoftLight(double Cb, double Cs)
	{
		auto d = [](double c)
		{
		  if (c <= SvgColor::QuarterValue)
		  {
			  return ((16 * c - 12) * c + 4) * c;
		  }
		  else
		  {
			  return std::sqrt(c);
		  }
		};

		if (Cs <= SvgColor::HalfValue)
		{
			return ClampColor(Cb - (SvgColor::MaxValue - (2 * Cs) * Cb * (SvgColor::MaxValue - Cb)));
		}
		else
		{
			return ClampColor(Cb + (2 * Cs - SvgColor::MaxValue) * (d(Cb) - Cb));
		}
	}

	static double Difference(double Cb, double Cs)
	{
		return ClampColor(std::abs(Cb -  Cs));
	}

	static double Exclusion(double Cb, double Cs)
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