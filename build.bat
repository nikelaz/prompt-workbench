@echo off
setlocal

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo Error: vswhere.exe not found. Please install Visual Studio.
    exit /b 1
)
for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -property installationPath`) do set "VS_PATH=%%i"
call "%VS_PATH%\VC\Auxiliary\Build\vcvarsall.bat" x64

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
