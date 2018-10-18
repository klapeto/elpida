pluginInstallDir = join_paths(binaryInstallPath, 'Elpida')

conf_data = configuration_data()

conf_data.set('_elpida_plugin_install_path', '"' + pluginInstallDir + '"')

if qt5_dep.found()
	conf_data.set('_elpida_qt_enabled', '1')
endif

configure_file(	input: 'Config.hpp.in',
				output : 'Config.hpp',
				configuration : conf_data)
				
subdir('General')
subdir('Image')
				

