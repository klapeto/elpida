//
// Created by klapeto on 8/3/2024.
//

#include "Elpida/Svg/SvgCalculationContext.hpp"
#include "Elpida/Xml/XmlMap.hpp"
#include "Elpida/Svg/SvgNumber.hpp"
#include "Elpida/Svg/SvgElement.hpp"
#include "Elpida/Svg/SvgViewPort.hpp"

namespace Elpida
{
	double SvgCalculationContext::GetDpi() const
	{
		return _dpi;
	}

	const SvgViewBox& SvgCalculationContext::GetViewBox() const
	{
		return _viewBox.top().value;
	}

	void SvgCalculationContext::Push(const SvgElement& element)
	{
		_currentDepth++;

		for (auto& pair : element.GetProperties())
		{
			auto& stack = _stackedValues[pair.first];
			stack.push({pair.second, _currentDepth});
		}

		if (!element.GetTransform().IsIdentity())
		{
			_transforms.push_back({element.GetTransform(), _currentDepth});
		}

		if (element.GetName() == "svg")
		{
			SvgViewPort viewPort(element.GetProperties());

			_viewPort.push({ SvgCalculatedViewPort(
					viewPort.GetX().CalculateValue(*this, GetViewPort().GetWidth()),
					viewPort.GetY().CalculateValue(*this, GetViewPort().GetHeight()),
					viewPort.GetWidth().CalculateValue(*this, GetViewPort().GetWidth()),
					viewPort.GetHeight().CalculateValue(*this, GetViewPort().GetHeight())),
							 _currentDepth }
			);

			auto viewBox = SvgViewBox(element.GetProperties().GetValue("viewBox"));
			if (viewBox.IsValid())
			{
				_viewBox.push({viewBox, _currentDepth});
			}
			else
			{
				auto& latestViewPort =_viewPort.top().value;
				_viewBox.push({SvgViewBox(latestViewPort.GetX(), latestViewPort.GetY(), latestViewPort.GetWidth(), latestViewPort.GetHeight()), _currentDepth});
			}

		}
	}

	SvgCalculationContext::SvgCalculationContext(double rootFontSize, double dpi)
			:_rootFontSize(rootFontSize), _dpi(dpi), _currentDepth(1)
	{
		auto& stack = _stackedValues["font-size"];
		stack.push({std::to_string(rootFontSize), _currentDepth});
		_viewBox.push({SvgViewBox(0, 0, 0, 0), _currentDepth});
		_viewPort.push({SvgCalculatedViewPort(0, 0, 300, 150), _currentDepth});
	}

	void SvgCalculationContext::Pop()
	{
		_currentDepth--;
		for (auto& pair : _stackedValues)
		{
			auto& stack = pair.second;

			while (!stack.empty() && stack.top().index > _currentDepth)
			{
				stack.pop();
			}
		}
		while (!_viewBox.empty() && _viewBox.top().index > _currentDepth)
		{
			_viewBox.pop();
		}

		while (!_transforms.empty() && _transforms.back().index > _currentDepth)
		{
			_transforms.pop_back();
		}

		while (!_viewPort.empty() && _viewPort.top().index > _currentDepth)
		{
			_viewPort.pop();
		}
	}

	std::string_view SvgCalculationContext::GetValue(const std::string_view& name) const
	{
		auto itr = _stackedValues.find(name);
		if (itr != _stackedValues.end())
		{
			auto& stack = (*itr).second;
			if (stack.empty()) return "";
			return stack.top().value;
		}
		return "";
	}

	double SvgCalculationContext::GetNumber(const std::string_view& name) const
	{
		double value = 0.0;
		SvgNumber::TryParseNumber(GetValue(name), value);
		return value;
	}

	const SvgCalculatedViewPort& SvgCalculationContext::GetViewPort() const
	{
		return _viewPort.top().value;
	}

	double SvgCalculationContext::GetRootFontSize() const
	{
		return _rootFontSize;
	}

	SvgTransform SvgCalculationContext::CalculateTransform() const
	{
		SvgTransform transform;

		for (auto& thisTransform : _transforms)
		{
			transform.Multiply(thisTransform.value);
		}

		return transform;
	}
} // Elpida