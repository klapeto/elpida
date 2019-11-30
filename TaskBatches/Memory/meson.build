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
						join_paths('Read', 'MemoryRead.cpp'),
						join_paths('Read', 'MultiThreadMemoryChunksRead.cpp'),
						join_paths('Read', 'MultithreadMemoryChunksReadTaskBatch.cpp'),
						join_paths('Read', 'MemoryReadTaskFactory.cpp'),
						join_paths('Latency', 'MemoryLatency.cpp'),
						join_paths('Latency', 'MemoryLatencyTaskBatch.cpp'),
						'WorkingSetSizes.cpp']

install_headers([
				join_paths('Read', 'MemoryRead.hpp'),
				join_paths('Read', 'MultiThreadMemoryChunksRead.hpp'),
				join_paths('Read', 'MultithreadMemoryChunksReadTaskBatch.hpp'),
				join_paths('Read', 'MemoryReadTaskFactory.hpp'),
				join_paths('Latency', 'MemoryLatency.hpp'),
				join_paths('Latency', 'MemoryLatencyTaskBatch.hpp'),
				'WorkingSetSizes.hpp'],
				subdir: 'Elpida/TaskBatches/Memory')
				
if qt5_dep.found()

	memoryTasks_sources += [
		join_paths('Ui', 'MemoryTasksProperties.cpp'),
		join_paths('Ui', 'MemoryTasksPropertiesWithChart.cpp'),
		join_paths('Ui', 'MemoryBandwidthChart.cpp'),
		join_paths('Ui', 'MemoryLatencyChart.cpp'),
	]

	uiFiles = [
		
	]
	
	mocHeaders = [
		join_paths('Ui', 'MemoryTasksProperties.hpp'),
		join_paths('Ui', 'MemoryTasksPropertiesWithChart.hpp'),
		join_paths('Ui', 'MemoryBandwidthChart.hpp'),
		join_paths('Ui', 'MemoryLatencyChart.hpp'),
	]
	
	memoryTasks_sources += qt5.preprocess(
	  moc_headers : mocHeaders,
	  ui_files    : uiFiles,
	)
endif
				
memoryTasks = library('MemoryTasks', 
							[memoryTasks_sources],
							link_with: [elpida, generalTasks],
							cpp_args: ['-masm=att', '-march=native', '-O0'],
							install_dir: pluginInstallDir,
							dependencies: qt5_dep.found() ? qt5_dep : [],
							install: true,
							include_directories : [rootIncude])
				
