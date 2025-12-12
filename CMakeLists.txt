add_library(oled STATIC
    ssd1306_i2c.c
    oled.c
)

target_compile_definitions(oled PUBLIC
    $<$<BOOL:${SSD1306_PLATFORM_STM32}>:STM32F429xx>
    $<$<BOOL:${SSD1306_PLATFORM_STM32}>:USE_HAL_DRIVER>
)

target_include_directories(oled PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    $<$<BOOL:${SSD1306_PLATFORM_STM32}>:${CMAKE_SOURCE_DIR}/Core/Inc>
    $<$<BOOL:${SSD1306_PLATFORM_STM32}>:${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc>
    $<$<BOOL:${SSD1306_PLATFORM_STM32}>:${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include>
    $<$<BOOL:${SSD1306_PLATFORM_STM32}>:${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include>
)

# Se estiver em um ambiente com pico-sdk, linka com pico_stdlib
# especificamente para o target "oled". Em outras plataformas
# (por exemplo STM32), esta condição será falsa e a lib permanece
# independente.
if(TARGET pico_stdlib AND TARGET oled)
    target_link_libraries(oled PUBLIC
        pico_stdlib
    )
endif()
