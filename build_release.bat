@echo off

if not exist "build_release" mkdir build_release

pushd build_release

cl /Fe:meme_generator.exe ../src/*.c user32.lib gdi32.lib kernel32.lib comdlg32.lib

popd