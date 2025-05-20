# SPDX-License-Identifier: Apache-2.0
set(CPU_TYPE cortex-m4)  # e.g., M4 for STM32F4
set(FLASH_SIZE 1024K)
set(RAM_SIZE 320K)

# keep first
board_runner_args(stm32cubeprogrammer "--port=swd" "--reset-mode=hw")
board_runner_args(jlink "--device=STM32F413VG" "--speed=4000")

# keep first
include(${ZEPHYR_BASE}/boards/common/stm32cubeprogrammer.board.cmake)
include(${ZEPHYR_BASE}/boards/common/openocd.board.cmake)
include(${ZEPHYR_BASE}/boards/common/jlink.board.cmake)
