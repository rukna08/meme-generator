@echo off

if exist "build" pushd build

devenv /debugexe meme_generator.exe

popd