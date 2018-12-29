/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

#ifndef ELPIDAMANAGER_HPP
#define ELPIDAMANAGER_HPP

#include <sstream>

#include <Elpida/PluginLoader.hpp>
#include <Elpida/Types/String.hpp>
#include <Elpida/Utilities/NonCopyable.hpp>
#include <Elpida/Types/Map.hpp>
#include <Elpida/Types/Array.hpp>

namespace Elpida
{
	class QtTaskBatchWrapper;

	class ElpidaManager final: NonCopyable
	{
		public:

			String getLog() const
			{
				return _log.str();
			}

			void setPluginDirectory(const String& directory)
			{
				_batchesDirectory = directory;
			}

			const PluginLoader& getPluginLoader() const
			{
				return _batchLoader;
			}

			const Map<String, QtTaskBatchWrapper*>& getCreatedTaskBatches() const
			{
				return _createdTaskBatches;
			}

			void reloadTaskBatches();
			void destroyTaskBatches();

			ElpidaManager();
			~ElpidaManager();

		private:
			Map<String, QtTaskBatchWrapper*> _createdTaskBatches;
			PluginLoader _batchLoader;
			String _batchesDirectory;
			std::stringstream _log;
	};
}

#endif // ELPIDAMANAGER_HPP
