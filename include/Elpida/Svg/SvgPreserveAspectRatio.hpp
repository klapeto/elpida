//
// Created by klapeto on 11/12/2023.
//

#ifndef ELPIDA_SVG_SVGPRESERVEASPECTRATIO_HPP
#define ELPIDA_SVG_SVGPRESERVEASPECTRATIO_HPP

#include <string_view>

namespace Elpida
{

	enum class SvgAlignType
	{
		Meet,
		Slice
	};

	enum class SvgAxisAlignType
	{
		None,
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

		explicit SvgPreserveAspectRatio(std::string_view view);
	private:
		SvgAlignType _type;
		SvgAxisAlignType _alignX;
		SvgAxisAlignType _alignY;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGPRESERVEASPECTRATIO_HPP
