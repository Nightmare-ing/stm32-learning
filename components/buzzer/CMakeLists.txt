add_library(component_buzzer STATIC)

target_sources(component_buzzer PRIVATE
    src/buzzer.c
)

target_sources(component_buzzer PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES include/buzzer/buzzer.h
)

target_link_libraries(component_buzzer PRIVATE
    ${PROJECT_NAME}::project_options
    ${PROJECT_NAME}::compiler_warnings
    ${PROJECT_NAME}::target_abi
)

add_library(components::buzzer ALIAS component_buzzer)
