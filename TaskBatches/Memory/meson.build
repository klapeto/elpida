#-------------------------------------------------------------------------------
# Elpida - Benchmark library
# 
# Copyright (C) 2018  Ioannis Panagiotopoulos
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>
#-------------------------------------------------------------------------------

memoryTasks_sources = ['Plugin.cpp',
						'MemoryRead.cpp',
						'MemoryTaskBatch.cpp',
						'MultiThreadMemoryChunksRead.cpp',
						'MultiThreadMemoryRead.cpp']

install_headers(['MemoryRead.cpp',
				'MemoryTaskBatch.cpp',
				'MultiThreadMemoryChunksRead.hpp',
				'MultiThreadMemoryRead.hpp'],
				subdir: 'Elpida/TaskBatches/Memory')
				
if qt5_dep.found()

	memoryTasks_sources += [
		'MemoryTasksProperties.cpp'
	]

	uiFiles = [
		
	]
	
	mocHeaders = [
		'MemoryTasksProperties.hpp'
	]
	
	memoryTasks_sources += qt5.preprocess(
	  moc_headers : mocHeaders,
	  ui_files    : uiFiles,
	)
endif
				
memoryTasks = library('MemoryTasks', 
							[memoryTasks_sources],
							link_with: [elpida, generalTasks],
							cpp_args: defaultCppArgs,
							install_dir: pluginInstallDir,
							dependencies: qt5_dep.found() ? qt5_dep : [],
							install: true,
							include_directories : [rootIncude])
				
