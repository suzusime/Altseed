import os.path
import aceutils

aceutils.cdToScript()
aceutils.cd(r'../')

aceutils.rmdir(r'zlib_bin')
aceutils.rmdir(r'libpng_bin')

aceutils.wget(r'http://sourceforge.net/projects/libpng/files/libpng16/older-releases/1.6.6/libpng-1.6.6.tar.gz')
aceutils.wget(r'http://zlib.net/zlib-1.2.8.tar.gz')

aceutils.call(r'tar zxvf libpng-1.6.6.tar.gz')
aceutils.call(r'tar zxvf zlib-1.2.8.tar.gz')

aceutils.editCmakeForACE(r'zlib-1.2.8/CMakeLists.txt')
aceutils.editCmakeForACE(r'libpng-1.6.6/CMakeLists.txt')

aceutils.mkdir(r'zlib_bin')
aceutils.mkdir(r'libpng_bin')

if aceutils.isWin():
	print('already installed.')
else:
	aceutils.cd(r'zlib_bin')
	if aceutils.isMac():
		aceutils.call(r'cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX:PATH=../Dev "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../zlib-1.2.8/')
	else:
		aceutils.call(r'cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX:PATH=../Dev ../zlib-1.2.8/')
	aceutils.call(r'make install')
	aceutils.cd(r'../')

	aceutils.cd(r'libpng_bin')
	if aceutils.isMac():
		aceutils.copy(r'../libpng-1.6.6/scripts/pnglibconf.h.prebuilt',r'../libpng-1.6.6/pnglibconf.h')
		aceutils.call(r'cmake -G "Unix Makefiles" -D PNG_STATIC:BOOL=ON -D CMAKE_INSTALL_PREFIX:PATH=../Dev "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../libpng-1.6.6/')
	else:
		aceutils.call(r'cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX:PATH=../Dev ../libpng-1.6.6/')
	aceutils.call(r'make install')
	aceutils.cd(r'../')

	aceutils.rm(r'Dev/lib/libz.so')
	aceutils.rm(r'Dev/lib/libz.so.1')
	aceutils.rm(r'Dev/lib/libz.so.1.2.8')

	aceutils.rm(r'Dev/lib/libpng.so')
	aceutils.rm(r'Dev/lib/libpng16.so')
	aceutils.rm(r'Dev/lib/libpng16.so.16')
	aceutils.rm(r'Dev/lib/libpng16.so.16.6.0')

	aceutils.rm(r'Dev/lib/libz.dylib')
	aceutils.rm(r'Dev/lib/libz.1.dylib')
	aceutils.rm(r'Dev/lib/libz.1.2.8.dylib')

	aceutils.rm(r'Dev/lib/libpng.dylib')
	aceutils.rm(r'Dev/lib/libpng16.dylib')
	aceutils.rm(r'Dev/lib/libpng16.16.dylib')
	aceutils.rm(r'Dev/lib/libpng16.16.6.0.dylib')
