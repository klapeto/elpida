/*
 * Plugin.hpp
 *
 *  Created on: 26 Σεπ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_PLUGIN_HPP_
#define ELPIDA_PLUGIN_HPP_

#include "Elpida/Types/String.hpp"

namespace Elpida
{
	class Plugin
	{
		private:
			void* getFunctionPointerImpl(const String& functionName) const;
		public:

			template<typename T>
			inline T getFunctionPointer(const String& functionName) const
			{
				return (T) getFunctionPointerImpl(functionName);
			}

			Plugin(const String& libraryPath);
			~Plugin();
		private:
			void* _handle;
		public:

			Plugin(const Plugin&) = delete;
			Plugin& operator=(const Plugin&) = delete;

			Plugin(Plugin&& other)
			{
				this->_handle = other._handle;
				other._handle = nullptr;
			}
			Plugin& operator=(Plugin&& other)
			{
				this->_handle = other._handle;
				other._handle = nullptr;
				return *this;
			}
	};

} /* namespace Elpida */

#endif /* ELPIDA_PLUGIN_HPP_ */
