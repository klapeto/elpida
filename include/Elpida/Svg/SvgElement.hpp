//
// Created by klapeto on 20/12/2023.
//

#ifndef ELPIDA_SVG_SVGELEMENT_HPP
#define ELPIDA_SVG_SVGELEMENT_HPP

#include "SvgFill.hpp"
#include "SvgPathInstance.hpp"
#include "SvgStroke.hpp"
#include "Elpida/Svg/SvgTransform.hpp"
#include "Elpida/Xml/XmlElement.hpp"

namespace Elpida
{
	class SvgDocument;

	class SvgElement
	{
	public:
		[[nodiscard]]
		const std::string& GetId() const
		{
			return _id;
		}

		[[nodiscard]]
		const SvgTransform& GetTransform() const
		{
			return _transform;
		}

		[[nodiscard]]
		const XmlMap& GetProperties() const
		{
			return _properties;
		}

		[[nodiscard]]
		const std::vector<SvgElement>& GetChildren() const
		{
			return _children;
		}

		[[nodiscard]]
		const SvgFill& GetFill() const
		{
			return _fill;
		}

		[[nodiscard]]
		const SvgStroke& GetStroke() const
		{
			return _stroke;
		}

		[[nodiscard]]
		double GetOpacity() const
		{
			return _opacity;
		}

		[[nodiscard]]
		bool IsVisible() const
		{
			return _visible;
		}

		const std::vector<SvgPathInstance>& GetPaths() const
		{
			return _paths;
		}

		[[nodiscard]]
		const SvgBounds& GetBounds() const
		{
			return _bounds;
		}



		SvgElement() = default;
		explicit SvgElement(const XmlElement& element, SvgDocument& document);
		SvgElement(const SvgElement&) = delete;
		SvgElement(SvgElement&& other) noexcept = default;
		SvgElement& operator=(SvgElement&&) noexcept = default;
		virtual ~SvgElement() = default;

	private:
		std::string _id;
		SvgTransform _transform;
		XmlMap _properties;
		std::vector<SvgPathInstance> _paths;
		std::vector<SvgElement> _children;
		SvgFill _fill;
		SvgStroke _stroke;
		SvgBounds _bounds;
		double _opacity;
		bool _visible;

		static constexpr double Kappa = 0.5522847493;
		void ParseAsRectangle(const SvgDocument& document);
		void ParseAsPath();

	protected:
		template<typename T, typename TConverter> void ConditionallyAssignProperty(const std::string& name,
			T& targetValue,
			TConverter converter)
		{
			if (auto& value = _properties.GetValue(name); !value.empty())
			{
				targetValue = std::move(converter(value));
			}
		}

		template<typename T> void ConditionallyAssignProperty(const std::string& name, T& targetValue)
		{
			ConditionallyAssignProperty<T>(name, targetValue, [](const auto& s) { return T(s); });
		}


		void CalculateBounds();
	};
} // Elpida

#endif //ELPIDA_SVG_SVGELEMENT_HPP
