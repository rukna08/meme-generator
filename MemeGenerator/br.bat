@echo off

mkdir build
pushd build
cl -Zi ../main.c User32.lib Gdi32.lib Kernel32.lib
main.exe
popd