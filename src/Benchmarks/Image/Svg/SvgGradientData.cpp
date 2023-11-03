//
// Created by klapeto on 3/11/2023.
//

#include "SvgGradientData.hpp"
#include "SvgGradientUnits.hpp"
#include "SvgPaintType.hpp"
#include "SvgSpreadType.hpp"

namespace Elpida
{
	SvgGradientData::SvgGradientData(SvgPaintType type, const std::vector<SvgAttributePair>& attributes)
		: _type(type),
		  _spread(SvgSpreadType::PAD),
		  _units(SvgGradientUnits::OBJECT_SPACE)
	{
		if (_type == SvgPaintType::LINEAR_GRADIENT)
		{
			_linearData = {};
		}
		else if (_type == SvgPaintType::RADIAL_GRADIENT)
		{
			_radialData = {};
		}
		_transform.SetIdentity();

		for (auto& attribute : attributes)
		{
			if (attribute.GetName() == "id")
			{
				_id = attribute.GetValue();
			}
		}
	}
} // Elpida