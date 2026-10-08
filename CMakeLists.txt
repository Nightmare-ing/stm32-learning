cmake_minimum_required(VERSION 3.25...4.4)
project(stm32_template
    VERSION 1.0
    DESCRIPTION "Template for STM32 projects"
    LANGUAGES C ASM)
set(STM32_LEARNING_NAMESPACE stm32-template)

# Use more efficient FetchContent_MakeAvailable() call, which is introduced in CMake 3.30.
set(CMAKE_POLICY_DEFAULT_CMP0168 NEW)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Create compile_commands.json in the build directory and link it to the source directory
if(NOT ${CMAKE_BINARY_DIR} STREQUAL "${CMAKE_SOURCE_DIR}/build")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    file(CREATE_LINK
        ${CMAKE_BINARY_DIR}/compile_commands.json
        "${CMAKE_SOURCE_DIR}/build/compile_commands.json"
        SYMBOLIC
        COPY_ON_ERROR
    )
endif()

# ======================================================================
# Load CMake Helpers
# ======================================================================
include(stm32-utils)


# ======================================================================
# Core Interface Targets (Project Options, Warnings, and Machine ABI)
#
# Provides a three-pillar interface contract for all downstream targets:
#   1. ${PROJECT_NAME}::project_options      - Language standard (C11) & common codegen flags
#   2. ${PROJECT_NAME}::compiler_warnings    - Strict code quality & static diagnostic flags
#   3. ${PROJECT_NAME}::target_abi           - Target-dependent machine ISA/ABI bridge
#                                              (No-op on Host tests, bridges bsp::mcu_config on MCU)
# ======================================================================
# 1. Project-wide language standards and dead-code stripping optimizations
add_library(${PROJECT_NAME}_project_options INTERFACE)
add_library(${PROJECT_NAME}::project_options ALIAS ${PROJECT_NAME}_project_options)

# Bare-metal common flags for optimization
target_compile_options(${PROJECT_NAME}_project_options INTERFACE
    -ffunction-sections
    -fdata-sections # prepare for GC optimization
)

# 2. Strict diagnostic warnings (isolated from third-party vendor libraries)
add_library(${PROJECT_NAME}_compiler_warnings INTERFACE)
add_library(${PROJECT_NAME}::compiler_warnings ALIAS ${PROJECT_NAME}_compiler_warnings)

target_compile_options(${PROJECT_NAME}_compiler_warnings INTERFACE
    $<$<C_COMPILER_ID:GNU,Clang>:
        -Wall
        -Wextra
        -Wshadow
        -Wconversion
        -Wundef
    >
)

# 3. Machine architecture & ISA bridge (multiplexes seamlessly between MCU and Host)
add_library(${PROJECT_NAME}_target_abi INTERFACE)
add_library(${PROJECT_NAME}::target_abi
    ALIAS ${PROJECT_NAME}_target_abi
)


# ======================================================================
# Platform-independent Components
# ======================================================================
add_subdirectory(components/led)


# ======================================================================
# Platform-specific Targets
# ======================================================================
# load BSP (Board Support Package) and ELF App when cross-compiling
if(CMAKE_CROSSCOMPILING)
    # Create bsp::mcu_config and board adapters
    # Board adapters can now safely depend on component targets
    add_subdirectory(bsp)

    # Populate the previously empty architecture interface
    target_link_libraries(${PROJECT_NAME}_target_abi INTERFACE
        bsp::mcu_config
    )

    # Final consumer
    add_subdirectory(src)
endif()

