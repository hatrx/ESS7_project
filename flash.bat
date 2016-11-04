@echo off
taskkill /F /IM openocd.exe
if "%1"=="--embed" goto :embed
goto :noembed

:embed
cmd /C openocd -s "%OpenOCD%\scripts" -f "%OpenOCD%\scripts\board\stm32f4discovery.cfg" -c "init" -c "reset halt" -c "flash write_image erase ./build/src/OS" -c "reset halt"
goto :exit

:noembed
start "OpenOCD" cmd /C openocd -s "%OpenOCD%\scripts" -f "%OpenOCD%\scripts\board\stm32f4discovery.cfg" -c "init" -c "reset halt" -c "flash write_image erase ./build/src/OS" -c "reset halt"

:exit
if "%2"=="--exit-on-finish" EXIT