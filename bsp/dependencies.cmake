include(FetchContent)

# ARM CMSIS 6
FetchContent_Declare(
    arm_cmsis
    GIT_REPOSITORY https://github.com/ARM-software/CMSIS_6.git
    GIT_TAG v6.3.0
    GIT_SHALLOW TRUE
    # Because CMSIS lib doesn't have a CMakeLists.txt, those two following keywords won't work, but just put here for future reference
    SYSTEM
    EXCLUDE_FROM_ALL
)

# ST CMSIS Device F1
FetchContent_Declare(
    stm32_cmsis
    GIT_REPOSITORY https://github.com/STMicroelectronics/cmsis-device-${MCU_FAMILY_SHORT}.git
    GIT_TAG v4.3.5
    GIT_SHALLOW TRUE
    SYSTEM
    EXCLUDE_FROM_ALL
)

# ST HAL Driver
FetchContent_Declare(
    stm32_hal_driver
    GIT_REPOSITORY https://github.com/STMicroelectronics/${MCU_FAMILY_LOWER}xx-hal-driver.git
    GIT_TAG v1.1.10
    GIT_SHALLOW TRUE
    SYSTEM
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(arm_cmsis stm32_cmsis stm32_hal_driver)

# Targets
add_library(arm_cmsis INTERFACE)
target_include_directories(arm_cmsis SYSTEM INTERFACE
    ${arm_cmsis_SOURCE_DIR}/CMSIS/Core/Include
)
add_library(arm::cmsis ALIAS arm_cmsis)

add_library(stm32_cmsis INTERFACE)
target_include_directories(stm32_cmsis SYSTEM INTERFACE
    ${stm32_cmsis_SOURCE_DIR}/Include
)
target_compile_definitions(stm32_cmsis INTERFACE
    ${MCU_MODEL}
)
add_library(stm32::cmsis ALIAS stm32_cmsis)

