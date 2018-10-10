#ifndef ELPIDAMANAGER_HPP
#define ELPIDAMANAGER_HPP

#include <string>

#include "Elpida/PluginLoader.hpp"
#include "Elpida/Types/String.hpp"
#include "Elpida/Utilities/NonCopyable.hpp"

namespace Elpida
{
	class ElpidaManager final: NonCopyable
	{
		public:

			void setPluginDirectory(const String& directory)
			{
				_batchesDirectory = directory;
			}

			const PluginLoader& getPluginLoader() const
			{
				return _batchLoader;
			}

			void reloadPlugins();

			ElpidaManager();
			~ElpidaManager();
		private:
			PluginLoader _batchLoader;
			String _batchesDirectory;
	};
}

#endif // ELPIDAMANAGER_HPP
