//
// Created by klapeto on 7/3/2024.
//

#include "Elpida/Svg/SvgCompositor.hpp"
#include "Elpida/Svg/SvgColor.hpp"

namespace Elpida
{
	static double Composite(double as, double Cs, double ab, double Cb, double Fa, double Fb)
	{
		return as * Fa * Cs + ab * Fb * Cb;
	}

	static double Clear(double as, double Cs, double ab, double Cb)
	{
		return 0.0;
	}

	static double Copy(double as, double Cs, double ab, double Cb)
	{
		return Composite(as, Cs, ab, Cb, 1.0, 0.0);
	}

	static double Destination(double as, double Cs, double ab, double Cb)
	{
		return Composite(as, Cs, ab, Cb, 0.0, 1.0);
	}

	static double SourceOver(double as, double Cs, double ab, double Cb)
	{
		return Composite(as, Cs, ab, Cb, 1.0, 1.0 - as);
	}

	static double DestinationOver(double as, double Cs, double ab, double Cb)
	{
		return Composite(as, Cs, ab, Cb, 1.0 - ab, 1.0);
	}

	static double SourceIn(double as, double Cs, double ab, double Cb)
	{
		return Composite(as, Cs, ab, Cb,  ab, 0.0);
	}

	static double DestinationIn(double as, double Cs, double ab, double Cb)
	{
		return Composite(as, Cs, ab, Cb, 0.0, as);
	}

	static double SourceOut(double as, double Cs, double ab, double Cb)
	{
		return Composite(as, Cs, ab, Cb, 1.0 - ab, 0.0);
	}

	static double DestinationOut(double as, double Cs, double ab, double Cb)
	{
		return Composite(as, Cs, ab, Cb, 0.0, 1.0 - as);
	}

	static double SourceAtop(double as, double Cs, double ab, double Cb)
	{
		return Composite(as, Cs, ab, Cb, ab, 1.0 - as);
	}

	static double DestinationAtop(double as, double Cs, double ab, double Cb)
	{
		return Composite(as, Cs, ab, Cb, 1.0 - ab, as);
	}

	static double Xor(double as, double Cs, double ab, double Cb)
	{
		return Composite(as, Cs, ab, Cb, 1.0 - ab, 1.0 - as);
	}

	static double Lighter(double as, double Cs, double ab, double Cb)
	{
		return Composite(as, Cs, ab, Cb, 1.0, 1.0);
	}

	SvgCompositor::SvgCompositor(SvgCompositingMode mode)
	{
		switch (mode)
		{
		case SvgCompositingMode::Clear:
			_compositor = Clear;
			break;
		case SvgCompositingMode::Copy:
			_compositor = Copy;
			break;
		case SvgCompositingMode::Destination:
			_compositor = Destination;
			break;
		case SvgCompositingMode::SourceOver:
			_compositor = SourceOver;
			break;
		case SvgCompositingMode::DestinationOver:
			_compositor = DestinationOver;
			break;
		case SvgCompositingMode::SourceIn:
			_compositor = SourceIn;
			break;
		case SvgCompositingMode::DestinationIn:
			_compositor = DestinationIn;
			break;
		case SvgCompositingMode::SourceOut:
			_compositor = SourceOut;
			break;
		case SvgCompositingMode::DestinationOut:
			_compositor = DestinationOut;
			break;
		case SvgCompositingMode::SourceAtop:
			_compositor = SourceAtop;
			break;
		case SvgCompositingMode::DestinationAtop:
			_compositor = DestinationAtop;
			break;
		case SvgCompositingMode::Xor:
			_compositor = Xor;
			break;
		case SvgCompositingMode::Lighter:
			_compositor = Lighter;
			break;
		default:
			_compositor = SourceOver;
		}
	}
} // Elpida