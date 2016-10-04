@echo off
openocd -s "%OpenOCD%\scripts" -f "%OpenOCD%\scripts\board\stm32f4discovery.cfg" -c "init" -c "reset halt" -c "flash write_image erase ./build/DemoBoard" -c "reset run" 