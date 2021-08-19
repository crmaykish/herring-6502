cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(memulator)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(memulator
    main.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(memulator pico_stdlib)

pico_enable_stdio_usb(memulator 1)
pico_enable_stdio_uart(memulator 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(memulator)