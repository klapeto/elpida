gtest_dep = gpp.find_library('gtest', required: false)

if gtest_dep.found()


	testExec = executable('CommandParser.Tests', 'CommandParser.Test.cpp',
				link_with: elpida,
				dependencies: [gtest_dep, thread_dep],
				include_directories : [rootIncude])
	test('Command Parser Tests', testExec)
	
	
	testExec = executable('Event.Tests', 'Event.Test.cpp',
				link_with: elpida,
				dependencies: [gtest_dep, thread_dep],
				include_directories : [rootIncude])
	test('Event Tests', testExec)
	
else
	message('Google Test was not found. Tests will not be built.')
endif