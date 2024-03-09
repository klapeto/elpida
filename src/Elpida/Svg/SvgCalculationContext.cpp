//
// Created by klapeto on 8/3/2024.
//

#include "Elpida/Svg/SvgCalculationContext.hpp"
#include "Elpida/Xml/XmlMap.hpp"
#include "Elpida/Svg/SvgNumber.hpp"

namespace Elpida
{
	double SvgCalculationContext::GetDpi() const
	{
		return _dpi;
	}

	const SvgViewBox& SvgCalculationContext::GetViewBox() const
	{
		return _viewBox;
	}

	void SvgCalculationContext::Push(const XmlMap& properties)
	{
		for (auto& pair : properties)
		{
			auto& stack = _stackedValues[pair.first];
			stack.push(pair.second);
		}
		_currentDepth++;
	}

	SvgCalculationContext::SvgCalculationContext(const SvgViewBox& viewBox, double rootFontSize, double dpi)
			:_viewBox(viewBox), _rootFontSize(rootFontSize), _dpi(dpi), _currentDepth(1)
	{
		auto& stack = _stackedValues["font-size"];
		stack.push(std::to_string(rootFontSize));
	}

	void SvgCalculationContext::Pop()
	{
		for (auto& pair : _stackedValues)
		{
			auto& stack = pair.second;

			while (stack.size() > _currentDepth - 1)
			{
				stack.pop();
			}
		}
		_currentDepth--;
	}

	std::string_view SvgCalculationContext::GetValue(const std::string_view& name) const
	{
		auto itr = _stackedValues.find(name);
		if (itr != _stackedValues.end())
		{
			auto& stack = (*itr).second;
			if (stack.empty()) return "";
			return stack.top();
		}
		return "";
	}

	double SvgCalculationContext::GetNumber(const std::string_view& name) const
	{
		double value = 0.0;
		SvgNumber::TryParseNumber(GetValue(name), value);
		return value;
	}
} // Elpida