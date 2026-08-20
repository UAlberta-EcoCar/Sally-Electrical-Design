# Branch for the original FOC-BOARD

Source code is located inside V1/FOC_SRC_V1
This is a CMake based project, and should be able to run in `STM32CubeIDE` or `STM32CubeIDE for VSCode`.

## Setup (If using Make)
1. Move inside V1/FOC_SRC_V1
```bash
cd V1/FOC_SRC_V1
```
2. Run the setup script
```bash
bash setup.sh
```
3. Run makefile to flash to device
```bash
make all
make flash
```
