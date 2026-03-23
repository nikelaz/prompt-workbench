@echo off
setlocal

if "%VCPKG_ROOT%"=="" (
    echo Error: VCPKG_ROOT environment variable is not set.
    echo Please set VCPKG_ROOT to your vcpkg installation directory.
    exit /b 1
)

if not exist build mkdir build
cd build
cmake -G Ninja ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ^
    ..
cmake --build . -j %NUMBER_OF_PROCESSORS%
cd ..
build\bin\app.exe
