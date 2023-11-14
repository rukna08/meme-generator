@echo off

if not exist "build" mkdir build

pushd build

cl -Zi ../src/main.c user32.lib gdi32.lib kernel32.lib

popd