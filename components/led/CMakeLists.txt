add_library(component_led STATIC)

target_sources(component_led PRIVATE
    src/led.c
)

target_sources(component_led PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES include/led/led.h
)

target_link_libraries(component_led PRIVATE
    ${PROJECT_NAME}::project_options
    ${PROJECT_NAME}::compiler_warnings
    ${PROJECT_NAME}::target_abi
)

add_library(components::led ALIAS component_led)
