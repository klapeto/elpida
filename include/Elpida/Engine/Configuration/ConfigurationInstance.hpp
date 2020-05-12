//
// Created by klapeto on 10/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONINSTANCE_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONINSTANCE_HPP


namespace Elpida
{
	class ConfigurationSpecificationBase;
	class ConfigurationValueBase;

	class ConfigurationInstance
	{
	public:
		[[nodiscard]] const ConfigurationSpecificationBase& getConfigurationSpecification() const
		{
			return *_configurationSpecification;
		}

		[[nodiscard]] const ConfigurationValueBase* getValue() const
		{
			return _value;
		}

		[[nodiscard]] bool isFixed() const
		{
			return _fixed;
		}

		explicit ConfigurationInstance(const ConfigurationSpecificationBase& configurationSpecification,
			const ConfigurationValueBase* value, bool fixed)
			: _configurationSpecification(&configurationSpecification), _value(value), _fixed(fixed)
		{

		}
		ConfigurationInstance(ConfigurationInstance&& other);
		ConfigurationInstance& operator=(ConfigurationInstance&& other);
		virtual ~ConfigurationInstance();
	private:
		const ConfigurationSpecificationBase* _configurationSpecification;
		const ConfigurationValueBase* _value;
		bool _fixed;
		void move(ConfigurationInstance&& other);
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONINSTANCE_HPP
