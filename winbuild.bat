@echo off
cd build
bash -c "make"
cd ..

if "%1"=="--exit-on-finish" EXIT