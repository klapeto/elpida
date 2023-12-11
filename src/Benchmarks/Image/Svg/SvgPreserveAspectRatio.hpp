//
// Created by klapeto on 11/12/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGPRESERVEASPECTRATIO_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGPRESERVEASPECTRATIO_HPP

namespace Elpida
{

	enum class SvgAlignType
	{
		None,
		Meet,
		Slice
	};

	enum class SvgAxisAlignType
	{
		Min,
		Mid,
		Max
	};

	class SvgPreserveAspectRatio
	{
	public:
		SvgAlignType GetType() const
		{
			return _type;
		}
		SvgAxisAlignType GetAlignX() const
		{
			return _alignX;
		}
		SvgAxisAlignType GetAlignY() const
		{
			return _alignY;
		}

		SvgPreserveAspectRatio()
			: _type(SvgAlignType::Meet), _alignX(SvgAxisAlignType::Mid), _alignY(SvgAxisAlignType::Mid)
		{

		}
		SvgPreserveAspectRatio(SvgAlignType type, SvgAxisAlignType alignX, SvgAxisAlignType alignY)
			: _type(type), _alignX(alignX), _alignY(alignY)
		{
		}
	private:
		SvgAlignType _type;
		SvgAxisAlignType _alignX;
		SvgAxisAlignType _alignY;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGPRESERVEASPECTRATIO_HPP
