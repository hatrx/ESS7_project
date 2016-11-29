@echo off
cd build
bash -c "make -j8"
cd ..

if "%1"=="--exit-on-finish" EXIT