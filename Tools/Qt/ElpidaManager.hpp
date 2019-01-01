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

#include <Elpida/Types/String.hpp>
#include <Elpida/Utilities/NonCopyable.hpp>
#include <Elpida/Types/Map.hpp>
#include <Elpida/Types/Array.hpp>
#include "Elpida/SharedLibraryLoader.hpp"

namespace Elpida
{
	class QtTaskBatchWrapper;

	class ElpidaManager final: NonCopyable
	{
		public:

			String getLogDump() const
			{
				return _log.str();
			}

			void setTaskBatchesDirectory(const String& directory)
			{
				_batchesDirectory = directory;
			}

			const SharedLibraryLoader& getLibraryLoader() const
			{
				return _batchLoader;
			}

			const Map<String, QtTaskBatchWrapper*>& getCreatedTaskBatches() const
			{
				return _createdTaskBatches;
			}

			const String& getTaskBatchesOrderFile() const
			{
				return _batchesOrderFile;
			}

			void setTaskBatchesOrderFile(const String& batchesOrderFile)
			{
				_batchesOrderFile = batchesOrderFile;
			}

			void reloadTaskBatches();
			void destroyTaskBatches();

			ElpidaManager();
			~ElpidaManager();
		private:
			Map<String, QtTaskBatchWrapper*> _createdTaskBatches;
			SharedLibraryLoader _batchLoader;
			String _batchesDirectory;
			String _batchesOrderFile;
			std::stringstream _log;
	};
}

#endif // ELPIDAMANAGER_HPP
