# ======================================================================
# Automatically generate .hex and .bin files for STM32 targets after build
# ======================================================================
function(stm32_add_hex_bin_targets TARGET_NAME)
    set(HEX_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.hex")
    set(BIN_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.bin")

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        BYPRODUCTS ${HEX_FILE} ${BIN_FILE}

        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${TARGET_NAME}> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET_NAME}> ${BIN_FILE}
        COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE_NAME:${TARGET_NAME}>

        COMMENT "Generating ${HEX_FILE} and ${BIN_FILE} and printed memory usage:"
    )
endfunction()

