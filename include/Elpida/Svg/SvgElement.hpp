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
		const std::vector<std::unique_ptr<SvgElement>>& GetChildren() const
		{
			return _children;
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
		std::vector<std::unique_ptr<SvgElement>> _children;
	protected:
		template <typename T, typename TConverter>
		void ConditionallyAssignProperty(const std::string& name, T& targetValue, TConverter converter)
		{
			if (auto& value = _properties.GetValue(name); !value.empty())
			{
				targetValue = std::move(converter(value));
			}
		}

		template <typename T>
		void ConditionallyAssignProperty(const std::string& name, T& targetValue)
		{
			ConditionallyAssignProperty<T>(name, targetValue, [](const auto& s) { return T(s); });
		}
	};
} // Elpida

#endif //ELPIDA_SVG_SVGELEMENT_HPP
