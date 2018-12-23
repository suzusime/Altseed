mkdir Log
python Script\build_ap.py > Log\ap.txt
python Script\build_ar.py > Log\ar.txt
python Script\build_glfw.py > Log\glfw.txt
python Script\build_glew.py > Log\glew.txt
python Script\build_Box2D.py > Log\Box2D.txt
python Script\build_bullet.py > Log\bullet.txt
python Script\build_gtest.py > Log\gtest.txt
python Script\build_effekseer.py > Log\effekseer.txt
python Script\build_OpenSoundMixer.py > Log\OpenSoundMixer.txt
python Script\build_freetype.py > Log\freetype.txt
python Script\build_libgd.py > Log\libgd.txt
python Script\build_culling2d.py > Log\culling2d.txt
python Script\build_culling3d.py > Log\culling3d.txt

python Dev\generateCoreToEngineHeader.py > Log\generateCoreToEngineHeader.txt
python Dev\generateCoreToEngineHeader.py > Log\generateCoreToEngineHeader.txt
python Dev\generate_swig.py > Log\generate_swig.txt
python Script\export_doxygen_core.py > Log\export_doxygen_core.txt
Script\generateSwigWrapper.py > Log\generateSwigWrapper.txt