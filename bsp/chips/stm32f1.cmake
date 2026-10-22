# ======================================================================
# Verified Dependency Tags for STM32F1 Family
# ======================================================================
set(STM32_CMSIS_DEVICE_TAG "v4.3.5" CACHE STRING "Git tag for cmsis-device-stm32f1 repository")
set(STM32_HAL_DRIVER_TAG "v1.1.10" CACHE STRING "Git tag for stm32f1xx-hal-driver repository")

# Preferred ARM CMSIS 6 tag
set(ARM_CMSIS_TAG "v6.3.0" CACHE STRING "Git tag for ARM CMSIS 6 repository")


# ======================================================================
# Flags for all static libraries and ELF
# ======================================================================
add_library(bsp_stm32f1_mcu_config INTERFACE)
# Cortex-M3 core and FPU configs
set(CPU_FLAGS
    -mcpu=cortex-m3
    -mthumb
    -mfloat-abi=soft
)

target_compile_options(bsp_stm32f1_mcu_config INTERFACE
    ${CPU_FLAGS}
)
target_link_options(bsp_stm32f1_mcu_config INTERFACE
    ${CPU_FLAGS}
)

add_library(bsp::mcu_config ALIAS bsp_stm32f1_mcu_config)
