//
// Created by klapeto on 7/3/2024.
//

#include "Elpida/Svg/SvgCompositor.hpp"

namespace Elpida
{
	static SvgFloat Composite(const SvgFloat as, const SvgFloat Cs, const SvgFloat ab, const SvgFloat Cb, const SvgFloat Fa, const SvgFloat Fb)
	{
		return (as * Fa * Cs) + (ab * Fb * Cb);
	}

	static SvgFloat Clear(SvgFloat as, SvgFloat Cs, SvgFloat ab, SvgFloat Cb)
	{
		return 0.0;
	}

	static SvgFloat Copy(const SvgFloat as, const SvgFloat Cs, const SvgFloat ab, const SvgFloat Cb)
	{
		return Composite(as, Cs, ab, Cb, 1.0, 0.0);
	}

	static SvgFloat Destination(const SvgFloat as, const SvgFloat Cs, const SvgFloat ab, const SvgFloat Cb)
	{
		return Composite(as, Cs, ab, Cb, 0.0, 1.0);
	}

	static SvgFloat SourceOver(const SvgFloat as, const SvgFloat Cs, const SvgFloat ab, const SvgFloat Cb)
	{
		return Composite(as, Cs, ab, Cb, 1.0, SvgFloat(1.0) - as);
	}

	static SvgFloat DestinationOver(const SvgFloat as, const SvgFloat Cs, const SvgFloat ab, const SvgFloat Cb)
	{
		return Composite(as, Cs, ab, Cb, SvgFloat(1.0) - ab, 1.0);
	}

	static SvgFloat SourceIn(const SvgFloat as, const SvgFloat Cs, const SvgFloat ab, const SvgFloat Cb)
	{
		return Composite(as, Cs, ab, Cb,  ab, 0.0);
	}

	static SvgFloat DestinationIn(const SvgFloat as, const SvgFloat Cs, const SvgFloat ab, const SvgFloat Cb)
	{
		return Composite(as, Cs, ab, Cb, 0.0, as);
	}

	static SvgFloat SourceOut(const SvgFloat as, const SvgFloat Cs, const SvgFloat ab, const SvgFloat Cb)
	{
		return Composite(as, Cs, ab, Cb, SvgFloat(1.0) - ab, 0.0);
	}

	static SvgFloat DestinationOut(const SvgFloat as, const SvgFloat Cs, const SvgFloat ab, const SvgFloat Cb)
	{
		return Composite(as, Cs, ab, Cb, 0.0, SvgFloat(1.0) - as);
	}

	static SvgFloat SourceAtop(const SvgFloat as, const SvgFloat Cs, const SvgFloat ab, const SvgFloat Cb)
	{
		return Composite(as, Cs, ab, Cb, ab, SvgFloat(1.0) - as);
	}

	static SvgFloat DestinationAtop(const SvgFloat as, const SvgFloat Cs, const SvgFloat ab, const SvgFloat Cb)
	{
		return Composite(as, Cs, ab, Cb, SvgFloat(1.0) - ab, as);
	}

	static SvgFloat Xor(const SvgFloat as, const SvgFloat Cs, const SvgFloat ab, const SvgFloat Cb)
	{
		return Composite(as, Cs, ab, Cb, SvgFloat(1.0) - ab, SvgFloat(1.0) - as);
	}

	static SvgFloat Lighter(const SvgFloat as, const SvgFloat Cs, const SvgFloat ab, const SvgFloat Cb)
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