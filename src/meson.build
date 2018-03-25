#-------------------------------------------------------------------------------
# elpida - CPU benchmark tool
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
conf_data = configuration_data()

conf_data.set('_elpida_compiler_string', '"' + gpp.get_id() + ' ' + gpp.version() + '"')

if build_machine.system() == 'linux'
	conf_data.set('_elpida_linux', 1)
else
	if build_machine.system() == 'windows'
		conf_data.set('_elpida_windows', 1)
	endif
endif

conf_data.set('_elpida_version_string', '"' + meson.project_version() + '"')

configure_file(	input: 'Config.hpp.in',
				output : 'Config.hpp',
				configuration : conf_data)



subdir('Utilities')
subdir('Tasks')

core_sources = ['CpuFeature.cpp',
				'CpuInfo.cpp',
				'Exportable.cpp', 
				'MultiThreadTask.cpp',
				'Runner.cpp',
				'SingleThreadTask.cpp',
				'Task.cpp',
				'TaskBatch.cpp',
				'TaskMetrics.cpp',
				'TaskThread.cpp',
				'TaskThroughput.cpp',
				'Timer.cpp']

core = static_library('Core', 
						core_sources,
						include_directories : inc)
						
install_headers(['CpuFeature.hpp',
				'CpuInfo.hpp',
				'Exportable.hpp',
				'MultiThreadTask.hpp',
				'Runner.hpp',
				'SingleThreadTask.hpp',
				'Task.hpp',
				'TaskBatch.hpp',
				'TaskMetrics.hpp',
				'TaskThread.hpp',
				'TaskThroughput.hpp',
				'Timer.hpp'],
				subdir: 'Elpida')
				

