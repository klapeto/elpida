//
// Created by klapeto on 22/4/20.
//

#ifndef APPS_QT_CONFIGURATIONVIEWSPOOL_HPP
#define APPS_QT_CONFIGURATIONVIEWSPOOL_HPP

#include <stack>
#include <Elpida/Engine/Configuration/ConfigurationType.hpp>

namespace Elpida
{
	class ConfigurationViewBase;
	class ConfigurationValueBase;

	class ConfigurationViewsPool
	{
	public:
		ConfigurationViewBase* rentViewForConfiguration(const ConfigurationValueBase& spec);
		void returnView(ConfigurationViewBase* view);
		ConfigurationViewsPool();
		virtual ~ConfigurationViewsPool();
	private:
		std::stack<ConfigurationViewBase*> _fileViews;

		template<typename T>
		ConfigurationViewBase* getFromStack(std::stack<ConfigurationViewBase*>& stack)
		{
			if (stack.size() > 0)
			{
				auto value = stack.top();
				stack.pop();
				return value;
			}
			else
			{
				return new T();
			}
		}

		template<typename T>
		ConfigurationViewBase* fillPool(std::stack<ConfigurationViewBase*>& stack)
		{
			for (size_t i = 0; i < _initialPoolsSize; ++i)
			{
				stack.push(new T());
			}
		}

		static void destroyPool(std::stack<ConfigurationViewBase*>& stack);

		const size_t _initialPoolsSize = 5;
	};
}


#endif //APPS_QT_CONFIGURATIONVIEWSPOOL_HPP
