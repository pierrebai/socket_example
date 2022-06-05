@echo off
setlocal
cd %~dp0
git submodule init
git submodule update
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -DCMAKE_GENERATOR_TOOLSET=v142
