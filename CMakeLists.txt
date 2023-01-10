add_executable(camera
        camera.c
        )

# pull in common dependencies and additional i2c hardware support
target_link_libraries(camera pico_stdlib hardware_i2c)

# enable usb output, disable uart output
#pico_enable_stdio_usb(camera 1)
#pico_enable_stdio_uart(camera 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(camera)

# add url via pico_set_program_url
example_auto_set_url(camera)
