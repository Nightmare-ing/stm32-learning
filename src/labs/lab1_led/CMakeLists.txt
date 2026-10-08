set(_target ${PROJECT_NAME}_lab1_led)
add_executable(${_target}
    main.c
)
set_target_properties(${_target} PROPERTIES SUFFIX ".elf")

target_link_libraries(${_target} PRIVATE
    ${PROJECT_NAME}::project_options
    ${PROJECT_NAME}::compiler_warnings
    bsp::runtime
    bsp::led
)
set(_map_file "$<TARGET_FILE_DIR:${_target}>/$<TARGET_PROPERTY:NAME>.map")
target_link_options(${_target} PRIVATE
    -Wl,--print-memory-usage
    "-Wl,-Map=${_map_file}"
)
set_property(TARGET ${_target} APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${_map_file}")

stm32_add_hex_bin_targets(${_target})
