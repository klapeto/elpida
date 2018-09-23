gtest_dep = gpp.find_library('gtest', required: false)

if gtest_dep.found()

	e = executable('CommandParser.test', 'CommandParser.Test.cpp',
				link_with: elpida,
				dependencies: [gtest_dep],
				include_directories : [rootIncude],)
	test('CommandParser Test', e)
else
	message('Google Test was not found. Tests will not be built.')
endif