@echo off
if not exist "build" mkdir build
cd build
cmake ..
cmake --build .
echo The assembly is complete! the executable file is located in the folder build/Release/memory_manager.exe
pause