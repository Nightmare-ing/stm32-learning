add_library(component_button STATIC)

target_sources(component_button PRIVATE
    src/button.c
)

target_sources(component_button PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES include/button/button.h
)

target_link_libraries(component_button PRIVATE
    ${PROJECT_NAME}::project_options
    ${PROJECT_NAME}::compiler_warnings
    ${PROJECT_NAME}::target_abi
)

add_library(components::button ALIAS component_button)
