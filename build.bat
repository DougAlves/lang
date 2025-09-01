@echo off

call "c:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/vcvarsall.bat" x64

mkdir .\build
pushd build
cl /Zi ..\src\main.cpp -o text.exe

mv text.exe ..\
popd
