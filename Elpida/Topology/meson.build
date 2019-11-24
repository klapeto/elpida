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
topology_sources = ['SystemTopology.cpp',
					'ProcessorNode.cpp']
					
topologyHeaders = ['SystemTopology.hpp',
					'ProcessorNode.hpp'
				]

topology = static_library('Topology', 
						topology_sources,
						cpp_args: defaultCppArgs,
						dependencies: numa_dep,
						link_whole: hwloc,
						include_directories : [rootIncude, hwlocInclude])
						
install_headers(topologyHeaders,
				subdir: 'Elpida/Topology')