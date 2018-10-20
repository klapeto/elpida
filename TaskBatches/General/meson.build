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

localCppArgs = ['-march=native', '-O3']

generalTasks_sources = ['ReadFile.cpp', 
						'WriteFile.cpp',
						'Plugin.cpp',
						'AllocateMemory.cpp']

generalTasks = library('GeneralTasks', 
							generalTasks_sources,
							link_with: elpida,
							install: true,
							cpp_args: localCppArgs,
							install_dir: pluginInstallDir,
							include_directories : rootIncude)
							
install_headers(['ReadFile.hpp',
				'WriteFile.hpp',
				'AllocateMemory.hpp'],
				subdir: 'Elpida/TaskBatches/General')

