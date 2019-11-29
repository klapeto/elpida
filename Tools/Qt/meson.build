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

if qt5_dep.found()

	elpidaQt_sources = [
		'Main.cpp',
		'MainWindow.cpp',
		'TaskBatchProperties.cpp',
		'ElpidaManager.cpp',
		'TaskBatchesWidget.cpp',
		'ListItemWithButton.cpp',
		'LogsDialog.cpp',
		'TopologyWidget.cpp',
		'MascotWidget.cpp',
		'TopologyFrame.cpp'
	]
	
	uiFiles = [
		'MainWindow.ui', 
		'TaskBatchProperties.ui',
		'TaskBatchesWidget.ui',
		'ListItemWithButton.ui',
		'LogsDialog.ui',
		'TopologyWidget.ui',
		'TopologyFrame.ui'
	]
	
	mocHeaders = [
		'MainWindow.hpp',
		'TaskBatchProperties.hpp',
		'TaskBatchesWidget.hpp',
		'ListItemWithButton.hpp',
		'LogsDialog.hpp',
		'TopologyWidget.hpp',
		'MascotWidget.hpp',
		'TopologyFrame.hpp'
	]
	
	elpidaQt_includes = include_directories('.')
	
	processed = qt5.preprocess(
	  moc_headers : mocHeaders,
	  ui_files    : uiFiles,
	  qresources  : 'Images.qrc',
	)
	
	executable('elpida-qt', sources: [elpidaQt_sources, processed],
				link_with: elpida,
				include_directories : [rootIncude, elpidaQt_includes],
				dependencies : [qt5_dep, thread_dep],
				cpp_args: defaultCppArgs,
				gui_app: true,
				build_rpath: libraryInstallPath + ':' + pluginInstallDir + ':' + binaryInstallPath,
				install_rpath: libraryInstallPath + ':' + pluginInstallDir + ':' + binaryInstallPath,
				install: true)
				
else
	warning('Qt5 was not found. The Qt front end will not be built.')
endif	
			
