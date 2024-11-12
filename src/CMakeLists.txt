set(SRCS "sdl/graphics.cpp"
         "sdl/keyboardController.cpp"
         "sdl/sound.cpp"
         "bus.cpp"
         "cpu.cpp"
         "emulator.cpp"
         "machine.cpp"
         "main.cpp"
         "memory.cpp"
)

add_executable(${PROJECT_NAME} ${SRCS})

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -pedantic
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
)
