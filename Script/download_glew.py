import aceutils

pathname = 'glew'

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):
	aceutils.rmdir(r"glew_bin")
	aceutils.rmdir(r"glew_bin_x64")

	aceutils.editCmakeForACE(pathname + r'/build/cmake/CMakeLists.txt')
	aceutils.mkdir(r"glew_bin")
	aceutils.mkdir(r"glew_bin_x64")

	with aceutils.CurrentDir('glew_bin'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake+r'-UGLEW_USE_STATIC_LIBS ../' + pathname + '/build/cmake')
			aceutils.call(aceutils.cmd_compile + r'ALL_BUILD.vcxproj /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'ALL_BUILD.vcxproj /p:configuration=Release')
		elif aceutils.isMac():
			aceutils.call(r'cmake -G "Unix Makefiles" -UGLEW_USE_STATIC_LIBS -D CMAKE_INSTALL_PREFIX:PATH=../Dev "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../' + pathname + '/build/cmake')
			aceutils.call(r'make install')
		else:
			aceutils.call(r'cmake -G "Unix Makefiles" -UGLEW_USE_STATIC_LIBS -D CMAKE_INSTALL_PREFIX=../Dev ../' + pathname + '/')
			aceutils.call(r'make install')

	with aceutils.CurrentDir('glew_bin_x64'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake_x64+r'-UGLEW_USE_STATIC_LIBS ../' + pathname + '/build/cmake')
			aceutils.call(aceutils.cmd_compile + r'ALL_BUILD.vcxproj /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'ALL_BUILD.vcxproj /p:configuration=Release')

	aceutils.mkdir(r"../Dev/include/GLFW/")

	if aceutils.isWin():
		aceutils.mkdir(r'../Dev/lib/x86/')
		aceutils.mkdir(r'../Dev/lib/x86/Debug')
		aceutils.mkdir(r'../Dev/lib/x86/Release')

		aceutils.mkdir(r'../Dev/lib/x64/')
		aceutils.mkdir(r'../Dev/lib/x64/Debug')
		aceutils.mkdir(r'../Dev/lib/x64/Release')

		aceutils.mkdir(r'../Dev/include')
		aceutils.mkdir(r'../Dev/include/GL')

		aceutils.copy(pathname + r'/include/GL/glew.h', r'../Dev/include/GL/')
		aceutils.copy(pathname + r'/include/GL/glxew.h', r'../Dev/include/GL/')
		aceutils.copy(pathname + r'/include/GL/wglew.h', r'../Dev/include/GL/')

		aceutils.copy(r'glew_bin/lib/Debug/libglew32d.lib', r'../Dev/lib/x86/Debug/')
		aceutils.copy(r'glew_bin/lib/Release/libglew32.lib', r'../Dev/lib/x86/Release/')

		aceutils.copy(r'glew_bin_x64/lib/Debug/libglew32d.lib', r'../Dev/lib/x64/Debug/')
		aceutils.copy(r'glew_bin_x64/lib/Release/libglew32.lib', r'../Dev/lib/x64/Release/')
