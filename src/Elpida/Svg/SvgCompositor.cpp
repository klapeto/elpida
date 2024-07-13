//
// Created by klapeto on 7/3/2024.
//

#include "Elpida/Svg/SvgCompositor.hpp"

namespace Elpida
{
	static double Composite(const double as, const double Cs, const double ab, const double Cb, const double Fa, const double Fb)
	{
		return (as * Fa * Cs) + (ab * Fb * Cb);
	}

	static double Clear(double as, double Cs, double ab, double Cb)
	{
		return 0.0;
	}

	static double Copy(const double as, const double Cs, const double ab, const double Cb)
	{
		return Composite(as, Cs, ab, Cb, 1.0, 0.0);
	}

	static double Destination(const double as, const double Cs, const double ab, const double Cb)
	{
		return Composite(as, Cs, ab, Cb, 0.0, 1.0);
	}

	static double SourceOver(const double as, const double Cs, const double ab, const double Cb)
	{
		return Composite(as, Cs, ab, Cb, 1.0, 1.0 - as);
	}

	static double DestinationOver(const double as, const double Cs, const double ab, const double Cb)
	{
		return Composite(as, Cs, ab, Cb, 1.0 - ab, 1.0);
	}

	static double SourceIn(const double as, const double Cs, const double ab, const double Cb)
	{
		return Composite(as, Cs, ab, Cb,  ab, 0.0);
	}

	static double DestinationIn(const double as, const double Cs, const double ab, const double Cb)
	{
		return Composite(as, Cs, ab, Cb, 0.0, as);
	}

	static double SourceOut(const double as, const double Cs, const double ab, const double Cb)
	{
		return Composite(as, Cs, ab, Cb, 1.0 - ab, 0.0);
	}

	static double DestinationOut(const double as, const double Cs, const double ab, const double Cb)
	{
		return Composite(as, Cs, ab, Cb, 0.0, 1.0 - as);
	}

	static double SourceAtop(const double as, const double Cs, const double ab, const double Cb)
	{
		return Composite(as, Cs, ab, Cb, ab, 1.0 - as);
	}

	static double DestinationAtop(const double as, const double Cs, const double ab, const double Cb)
	{
		return Composite(as, Cs, ab, Cb, 1.0 - ab, as);
	}

	static double Xor(const double as, const double Cs, const double ab, const double Cb)
	{
		return Composite(as, Cs, ab, Cb, 1.0 - ab, 1.0 - as);
	}

	static double Lighter(const double as, const double Cs, const double ab, const double Cb)
	{
		return Composite(as, Cs, ab, Cb, 1.0, 1.0);
	}

	SvgCompositor::SvgCompositor(const SvgCompositingMode mode)
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