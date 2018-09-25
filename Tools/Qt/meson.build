qt5 = import('qt5')
qt5_dep = dependency('qt5', modules: ['Widgets'], required : false)

if qt5_dep.found()

elpidaQt_sources = [
	'Main.cpp',
	'MainWindow.cpp',
	'TaskBatchProperties.cpp'
]

uiFiles = [
	'MainWindow.ui', 
	'TaskBatchProperties.ui'		
]

mocHeaders = [
	'MainWindow.hpp',
	'TaskBatchProperties.hpp'
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
			dependencies : qt5_dep,
			build_rpath: libraryInstallPath + ':' + pluginInstallDir,
			install_rpath: libraryInstallPath + ':' + pluginInstallDir,
			install: true)
else
	warning('Qt5 was not found. The Qt front end will not be built.')
endif	
			