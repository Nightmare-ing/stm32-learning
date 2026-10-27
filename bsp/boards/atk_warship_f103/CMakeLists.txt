# Basic board initialization
add_library(bsp_atk_warship_f103_core STATIC)

target_sources(bsp_atk_warship_f103_core PRIVATE
    core/board_clock.c
)
target_sources(bsp_atk_warship_f103_core PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES include/board/board.h
)

target_link_libraries(bsp_atk_warship_f103_core
    PRIVATE
        ${PROJECT_NAME}::project_options
        ${PROJECT_NAME}::compiler_warnings
        ${PROJECT_NAME}::target_abi
        stm32_hal
)

add_library(bsp::board_core ALIAS bsp_atk_warship_f103_core)

# Led Adapter
add_library(board_atk_warship_f103_led STATIC)

target_sources(board_atk_warship_f103_led PRIVATE
    adapters/led.c
)
target_sources(board_atk_warship_f103_led PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES include/board/led.h
)

target_link_libraries(board_atk_warship_f103_led
    PUBLIC
        components::led
    PRIVATE
        ${PROJECT_NAME}::project_options
        ${PROJECT_NAME}::compiler_warnings
        ${PROJECT_NAME}::target_abi
        stm32_hal
)

add_library(bsp::led ALIAS board_atk_warship_f103_led)

# Button Adapter
add_library(board_atk_warship_f103_button STATIC)

target_sources(board_atk_warship_f103_button PRIVATE
    adapters/button.c
)
target_sources(board_atk_warship_f103_button PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES include/board/button.h
)

target_link_libraries(board_atk_warship_f103_button
    PUBLIC
        components::button
    PRIVATE
        ${PROJECT_NAME}::project_options
        ${PROJECT_NAME}::compiler_warnings
        ${PROJECT_NAME}::target_abi
        stm32_hal
)

add_library(bsp::button ALIAS board_atk_warship_f103_button)

# Buzzer Adapter
add_library(board_atk_warship_f103_buzzer STATIC)

target_sources(board_atk_warship_f103_buzzer PRIVATE
    adapters/buzzer.c
)
target_sources(board_atk_warship_f103_buzzer PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES include/board/buzzer.h
)

target_link_libraries(board_atk_warship_f103_buzzer
    PUBLIC
        components::buzzer
    PRIVATE
        ${PROJECT_NAME}::project_options
        ${PROJECT_NAME}::compiler_warnings
        ${PROJECT_NAME}::target_abi
        stm32_hal
)

add_library(bsp::buzzer ALIAS board_atk_warship_f103_buzzer)
