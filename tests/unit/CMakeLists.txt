# Build unit tests for the led component
add_executable(test_led
    test_led.c
)

target_link_libraries(test_led PRIVATE
    ${PROJECT_NAME}::project_options
    ${PROJECT_NAME}::compiler_warnings
    ${PROJECT_NAME}::target_abi
    components::led
)

add_test(NAME test_led COMMAND test_led)

# Build unit tests for the button component
add_executable(test_button
    test_butn.c
)

target_link_libraries(test_button PRIVATE
    ${PROJECT_NAME}::project_options
    ${PROJECT_NAME}::compiler_warnings
    ${PROJECT_NAME}::target_abi
    components::button
)

add_test(NAME test_button COMMAND test_button)

