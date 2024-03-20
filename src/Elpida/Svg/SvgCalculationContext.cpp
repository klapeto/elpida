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
		return _viewBox.top();
	}

	void SvgCalculationContext::Push(const SvgElement& element)
	{
		for (auto& pair : element.GetProperties())
		{
			auto& stack = _stackedValues[pair.first];
			stack.push(pair.second);
		}

		if (!element.GetTransform().IsIdentity())
		{
			_transforms.push_back(element.GetTransform());
		}

		if (element.GetName() == "svg")
		{
			SvgViewPort viewPort(element.GetProperties());

			_viewPort.emplace(
					viewPort.GetX().CalculateValue(*this, GetViewPort().GetWidth()),
					viewPort.GetY().CalculateValue(*this, GetViewPort().GetHeight()),
					viewPort.GetWidth().CalculateValue(*this, GetViewPort().GetWidth()),
					viewPort.GetHeight().CalculateValue(*this, GetViewPort().GetHeight())
			);

			auto viewBox = SvgViewBox(element.GetProperties().GetValue("viewBox"));
			if (viewBox.IsValid())
			{
				_viewBox.push(viewBox);
			}
			else
			{
				auto& latestViewPort =_viewPort.top();
				_viewBox.emplace(latestViewPort.GetX(), latestViewPort.GetY(), latestViewPort.GetWidth(), latestViewPort.GetHeight());
			}

		}

		_currentDepth++;
	}

	SvgCalculationContext::SvgCalculationContext(double rootFontSize, double dpi)
			:_rootFontSize(rootFontSize), _dpi(dpi), _currentDepth(1)
	{
		auto& stack = _stackedValues["font-size"];
		stack.push(std::to_string(rootFontSize));
		_viewBox.emplace(0, 0, 0, 0);
		_viewPort.emplace(0, 0, 300, 150);
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
		while (_viewBox.size() > _currentDepth - 1)
		{
			_viewBox.pop();
		}

		while (_transforms.size() > _currentDepth - 1)
		{
			_transforms.pop_back();
		}

		while (_viewPort.size() > _currentDepth - 1)
		{
			_viewPort.pop();
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

	const SvgCalculatedViewPort& SvgCalculationContext::GetViewPort() const
	{
		return _viewPort.top();
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
			transform.PreMultiply(thisTransform);
		}

		return transform;
	}
} // Elpida