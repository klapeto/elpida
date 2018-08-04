pluginInstallDir = join_paths(libraryInstallPath, 'Elpida')

conf_data = configuration_data()

conf_data.set('_elpida_plugin_install_path', '"' + pluginInstallDir + '"')

configure_file(	input: 'Config.hpp.in',
				output : 'Config.hpp',
				configuration : conf_data)

subdir('General')
subdir('Image')
