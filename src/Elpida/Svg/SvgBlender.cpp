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
		if (color > 1.0) color = 1.0;
		if (color < 0.0) color = 0.0;
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
		if (Cs <= 0.5)
		{
			return Multiply(Cb, 2 * Cs);
		}
		else
		{
			return Screen(Cb, (2 * Cs) - 1.0);
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
			return ClampColor(std::min(1.0, Cb / (1.0 - Cs)));
		}
	}

	static double ColorBurn(double Cb, double Cs)
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
			return ClampColor(1.0 - std::min(1.0, (1.0 - Cb) / Cs));
		}
	}

	static double SoftLight(double Cb, double Cs)
	{
		auto d = [](double c)
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
			return ClampColor(Cb - (1.0 - 2 * Cs) * Cb * (1.0 - Cb));
		}
		else
		{
			return ClampColor(Cb + (2 * Cs - 1.0) * (d(Cb) - Cb));
		}
	}

	static double Difference(double Cb, double Cs)
	{
		return ClampColor(std::abs(Cb - Cs));
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