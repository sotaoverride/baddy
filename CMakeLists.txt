add_executable(camera
        camera.c
        )

# pull in common dependencies
target_link_libraries(camera pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(camera)

# add url via pico_set_program_url
example_auto_set_url(camera)
