# ======================================================================
# Specify BOARD MCU Family and Model, fetch dependencies
# ======================================================================
if(NOT DEFINED BOARD)
    message(FATAL_ERROR "BOARD is not defined. Please specify a board in CMake Presets or CLI.")
endif()

set(BOARD_DIR "${CMAKE_CURRENT_LIST_DIR}/boards/${BOARD}")
if(NOT EXISTS "${BOARD_DIR}/board.cmake")
    message(FATAL_ERROR "Unknown BOARD: ${BOARD}. No board.cmake found in ${BOARD_DIR}")
endif()
include("${BOARD_DIR}/board.cmake")


set(MCU_FAMILY ${BOARD_MCU_FAMILY})
set(MCU_MODEL ${BOARD_MCU_MODEL})
set(STM32_DRIVER_TYPE "LL" CACHE STRING "Driver type: LL, HAL, BARE, MIXED")

string(TOLOWER ${MCU_FAMILY} _mcu_family_lower)
set(MCU_FAMILY_LOWER ${_mcu_family_lower} CACHE INTERNAL "Lowercase MCU family")
string(SUBSTRING ${MCU_FAMILY_LOWER} 5 -1 _mcu_family_short) # e.g., extract "f1"
set(MCU_FAMILY_SHORT ${_mcu_family_short} CACHE INTERNAL "Short MCU family")
string(TOLOWER ${MCU_MODEL} _mcu_model_lower)
set(MCU_MODEL_LOWER ${_mcu_model_lower} CACHE INTERNAL "Lowercase MCU model")

include("${CMAKE_CURRENT_LIST_DIR}/dependencies.cmake")


# ======================================================================
# Chip Specific Configs
# ======================================================================
set(_chip_config_file "${CMAKE_CURRENT_LIST_DIR}/chips/${MCU_FAMILY_LOWER}.cmake")
if(EXISTS "${_chip_config_file}")
    message(STATUS "Including chip specific config: ${_chip_config_file}")
    include("${_chip_config_file}")
else()
    message(FATAL_ERROR "No chip specific config found for ${MCU_FAMILY_LOWER} at ${_chip_config_file}")
endif()


# ======================================================================
# Startup and Linker Script Configs
# ======================================================================
add_library(bsp_startup STATIC)
target_sources(bsp_startup PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/startup/startup_${MCU_MODEL_LOWER}.s"
    "${CMAKE_CURRENT_LIST_DIR}/startup/system_${MCU_FAMILY_LOWER}xx.c"
)
target_link_libraries(bsp_startup PUBLIC bsp::mcu_config stm32::cmsis arm::cmsis)
STRING(TOUPPER ${MCU_MODEL} _mcu_model_upper)
set(_linker_script "${CMAKE_CURRENT_LIST_DIR}/ldscripts/${_mcu_model_upper}_FLASH.ld")
target_link_options(bsp_startup INTERFACE "-T${_linker_script}")
set_target_properties(bsp_startup PROPERTIES INTERFACE_LINK_DEPENDS "${_linker_script}")
add_library(bsp::startup ALIAS bsp_startup)


# ======================================================================
# Common Baremetal Linker Configs for Cortex-M chips
# ======================================================================
add_library(bsp_baremetal_linker INTERFACE)
target_link_options(bsp_baremetal_linker INTERFACE
    --specs=nano.specs
    --specs=nosys.specs
    -Wl,--gc-sections
)
add_library(bsp::baremetal_linker ALIAS bsp_baremetal_linker)


# ======================================================================
# Driver Library: stm32::ll or stm32::hal
# ======================================================================
if(STM32_DRIVER_TYPE STREQUAL "LL")
    message(STATUS "Configuring project for ${MCU_MODEL} with LL driver")
    # LL Library: stm32::ll
    add_library(stm32_ll STATIC)
    target_link_libraries(stm32_ll PUBLIC stm32::cmsis arm::cmsis bsp::mcu_config)
    # Headers might couple with each other, so not use File Set to avoid missing headers
    target_include_directories(stm32_ll SYSTEM PUBLIC
        ${stm32_hal_driver_SOURCE_DIR}/Inc
        ${stm32_hal_driver_SOURCE_DIR}/Inc/Legacy
    )
    # Other consumers including those headers also need these macros
    target_compile_definitions(stm32_ll PUBLIC
        USE_FULL_LL_DRIVER
    )
    target_sources(stm32_ll PRIVATE
        ${stm32_hal_driver_SOURCE_DIR}/Src/${MCU_FAMILY_LOWER}xx_ll_gpio.c
        ${stm32_hal_driver_SOURCE_DIR}/Src/${MCU_FAMILY_LOWER}xx_ll_rcc.c
        ${stm32_hal_driver_SOURCE_DIR}/Src/${MCU_FAMILY_LOWER}xx_ll_usart.c
    )
    add_library(bsp::driver ALIAS stm32_ll)

elseif(STM32_DRIVER_TYPE STREQUAL "HAL")
    message(STATUS "Configuring project for ${MCU_MODEL} with HAL driver")
    # HAL Library: stm32::hal
    add_library(stm32_hal STATIC)
    target_link_libraries(stm32_hal PUBLIC stm32::cmsis arm::cmsis bsp::mcu_config)
    target_include_directories(stm32_hal SYSTEM PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/hal_config
        ${stm32_hal_driver_SOURCE_DIR}/Inc
        ${stm32_hal_driver_SOURCE_DIR}/Inc/Legacy
    )
    target_compile_definitions(stm32_hal PUBLIC
        USE_HAL_DRIVER
    )
    target_sources(stm32_hal PRIVATE
        ${stm32_hal_driver_SOURCE_DIR}/Src/${MCU_FAMILY_LOWER}xx_hal.c
        ${stm32_hal_driver_SOURCE_DIR}/Src/${MCU_FAMILY_LOWER}xx_hal_rcc.c
        ${stm32_hal_driver_SOURCE_DIR}/Src/${MCU_FAMILY_LOWER}xx_hal_gpio.c
        ${stm32_hal_driver_SOURCE_DIR}/Src/${MCU_FAMILY_LOWER}xx_hal_cortex.c
    )
    add_library(bsp::driver ALIAS stm32_hal)

elseif(STM32_DRIVER_TYPE STREQUAL "BARE")
    message(STATUS "Configuring project for ${MCU_MODEL} with bare-metal / CMSIS only")
    add_library(bsp_driver_bare INTERFACE)
    target_link_libraries(bsp_driver_bare INTERFACE stm32::cmsis arm::cmsis)
    add_library(bsp::driver ALIAS bsp_driver_bare)

else()
    message(FATAL_ERROR "Unsupported STM32_DRIVER_TYPE: ${STM32_DRIVER_TYPE}")
endif()


# ======================================================================
# BSP No-OS Runtime Environment
# Aggregates everything needed for ANY no-OS firmware to boot up
# ======================================================================
add_library(bsp_runtime INTERFACE)
target_link_libraries(bsp_runtime INTERFACE
    ${PROJECT_NAME}::target_abi
    bsp::baremetal_linker
    bsp::startup
    bsp::driver
)
add_library(bsp::runtime ALIAS bsp_runtime)


# ======================================================================
# Add Board Adapters
# ======================================================================
add_subdirectory(boards/${BOARD})
