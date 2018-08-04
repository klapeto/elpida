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
subdir('Encoders')

imageTasks_sources = ['GrayscaleAverage.cpp',
						'ImageTaskBatch.cpp',
						'PngDecoding.cpp',
						'PngEncoding.cpp',
						'FloydSteinberg.cpp',
						'ConvertToFloat.cpp',
						'ConvertToUInt8.cpp',
						'Plugin.cpp']

install_headers(['GrayscaleAverage.hpp',
						'ImageTaskBatch.hpp',
						'PngDecoding.hpp',
						'PngEncoding.hpp',
						'FloydSteinberg.hpp',
						'ConvertToFloat.hpp',
						'ConvertToUInt8.hpp'],
				subdir: 'Elpida/TaskBatches/Image')

imageTasks = library('ImageTasks', 
							imageTasks_sources,
							link_whole:imageEncoders,
							link_with: [elpida, generalTasks],
							install_dir: pluginInstallDir,
							install: true,
							include_directories : [rootIncude])

