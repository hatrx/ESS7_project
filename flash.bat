@echo off
taskkill /F /IM openocd.exe
start "OpenOCD" cmd /C openocd -s "%OpenOCD%\scripts" -f "%OpenOCD%\scripts\board\stm32f4discovery.cfg" -c "init" -c "reset halt" -c "flash write_image erase ./build/DemoBoard" -c "reset halt" 