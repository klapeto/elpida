//
// Created by klapeto on 20/12/2023.
//

#ifndef ELPIDA_SVG_SVGELEMENT_HPP
#define ELPIDA_SVG_SVGELEMENT_HPP

#include <memory>

#include "Elpida/Svg/SvgTransform.hpp"
#include "Elpida/Xml/XmlElement.hpp"

namespace Elpida
{
	class SvgDefs;

	class SvgElement
	{
	public:
		explicit SvgElement(const XmlElement& element, SvgDefs& defs);
		SvgElement(const SvgElement&) = delete;
		SvgElement(SvgElement&&) = default;
		virtual ~SvgElement() = default;
	private:
		std::string _id;
		SvgTransform _transform;
		XmlMap _properties;
		std::vector<std::unique_ptr<SvgElement>> _children;
	protected:

		template<typename T, typename TConverter>
		void ConditionallyAssignProperty(const std::string& name, T& targetValue, TConverter converter)
		{
			if (auto& value = _properties.GetValue(name); !value.empty())
			{
				targetValue = std::move(converter(value));
			}
		}

		template<typename T>
		void ConditionallyAssignProperty(const std::string& name, T& targetValue)
		{
			ConditionallyAssignProperty<T>(name, targetValue, [](const auto& s){return T(s);});
		}

		const XmlMap& GetProperties() const
		{
			return _properties;
		}
	};
} // Elpida

#endif //ELPIDA_SVG_SVGELEMENT_HPP
