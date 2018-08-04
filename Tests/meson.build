gtest_dep = gpp.find_library('gtest', required: true)

e = executable('CommandParser.test', 'CommandParser.Test.cpp',
			link_with: elpida,
			dependencies: [gtest_dep],
			include_directories : [rootIncude],)
test('CommandParser Test', e)