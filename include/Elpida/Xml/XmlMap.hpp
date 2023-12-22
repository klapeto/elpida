//
// Created by klapeto on 21/12/2023.
//

#ifndef ELPIDA_XML_XMLMAP_HPP
#define ELPIDA_XML_XMLMAP_HPP

#include <string>
#include <unordered_map>

namespace Elpida
{
	class XmlMap : public std::unordered_map<std::string_view, std::string>
	{
	public:
		const std::string& GetValue(const std::string_view key) const
		{
			if (const auto itr = find(key); itr != end())
			{
				return itr->second;
			}
			return Empty;
		}

		void Set(const std::string_view key, std::string value)
		{
			this->operator[](key) = std::move(value);
		}

		XmlMap() = default;

	private:
		static inline const std::string Empty;
	};
} // Elpida

#endif //ELPIDA_XML_MAP_HPP
